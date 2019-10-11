#include "Image.h"

namespace core
{

	Image::Image() :
		width(0), 
		height(0), 
		format(IF_UNDEFINED), 
		pixelFormat(PF_UNDEFINED), 
		pixelStructure(PS_UNDEFINED), 
		data(0)
	{};


	IMAGE_FORMAT Image::getFormat(const std::vector<unsigned char> _header)
	{
		// bmp files start with "BM" header
		if (_header.size() >= 2 && _header[0] == 0x42 && _header[1] == 0x4D)
			return IF_BMP;

		// jpg files start with 0xFF 0xD8 header
		if (_header.size() >= 2 && _header[0] == 0xFF && _header[1] == 0xD8)
			return IF_JPG;

		// png files start with 0x89 "PNG" header
		if (_header.size() >= 4 && _header[0] == 0x89 && _header[1] == 0x50 && _header[2] == 0x4E && _header[3] == 0x47)
			return IF_PNG;

		return IF_UNDEFINED;
	};


	void Image::convertBMPToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2)
	{
		assert(format == IF_BMP && "format conversion require BMP input");

		// make sure short is 2b and long int is 4b
		assert(sizeof(unsigned short) == 2 && "unsigned short size not equal to 2");
		assert(sizeof(unsigned long int) == 4 && "unsigned long int size not equal to 4");


		unsigned int updatedWidth = width;
		unsigned int updatedHeight = height;

		IMAGE_FORMAT updatedFormat = IF_RAW;
		PIXEL_FORMAT updatedPixelFormat = _newPixelFormat;
		PIXEL_STRUCTURE updatedPixelStructure = _newPixelStructure;

		std::vector<unsigned char> newData(0);


		if (_roundSizeToPow2)
		{
			updatedWidth = PO2(updatedWidth);
			updatedHeight = PO2(updatedHeight);
		}

		short newPixelSize = 0;
		if (_newPixelStructure & (PS_USHORT_4_4_4_4 | PS_USHORT_5_5_5_1) && _newPixelFormat & PF_RGBA)
			newPixelSize = 2;
		else if (_newPixelStructure & PS_USHORT_5_6_5 && _newPixelFormat & PF_RGB)
			newPixelSize = 2;
		else if (_newPixelStructure & PS_BYTE_PER_COMPONENT && _newPixelFormat & PF_RGB)
			newPixelSize = 3;
		else if (_newPixelStructure & PS_BYTE_PER_COMPONENT && _newPixelFormat & PF_RGBA)
			newPixelSize = 4;

		assert(newPixelSize && "pixel size cannot be 0");


		struct BMPHEADERS
		{
			// file header part
			unsigned short signature;
			unsigned long int fileSize;
			unsigned short reserved1, reserved2;
			unsigned long int offsetToPixelArray;

			// DIB header part
			unsigned long int dibHeaderSize;
			unsigned long int imageWidth, imageHeight;
			unsigned short planes, bitsPerPixel;
			unsigned long int compression, imageSize;
			// ...
		} fileHeader;

		memcpy(&fileHeader, &data[0], sizeof(BMPHEADERS));


		// BMP pixel array goeas by rows with padding so row length is multiple of 4
		unsigned short pixelArrayPadding = ((int((fileHeader.imageWidth - 1) * 0.25) + 1) * 4) - (fileHeader.imageWidth * 3);
		unsigned short pixelArrayRowLen = fileHeader.imageWidth + pixelArrayPadding;

		const unsigned char* pixelArray = &data[fileHeader.offsetToPixelArray];

		if (data.size() < fileHeader.offsetToPixelArray + fileHeader.imageSize)
			throw std::length_error("BMP size don't match with header information");


		//  from BGR_8_8_8
		if (fileHeader.bitsPerPixel == 24)
		{
			newData.resize(updatedWidth * updatedHeight * newPixelSize);

			//  BGR => RGB
			if (_newPixelStructure & PS_BYTE_PER_COMPONENT && _newPixelFormat & PF_RGB)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						newData[((updatedWidth * row + col) * 3) + 0] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 2];
						newData[((updatedWidth * row + col) * 3) + 1] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 1];
						newData[((updatedWidth * row + col) * 3) + 2] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 0];
					}
				}
			}

			//  BGR => RGBA
			else if (_newPixelStructure & PS_BYTE_PER_COMPONENT && _newPixelFormat & PF_RGBA)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						newData[((updatedWidth * row + col) * 4) + 0] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 2];
						newData[((updatedWidth * row + col) * 4) + 1] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 1];
						newData[((updatedWidth * row + col) * 4) + 2] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 0];
						newData[((updatedWidth * row + col) * 4) + 3] = 0xFF;
					}
				}
			}

			//  BGR => RGBA (short)
			else if (_newPixelStructure & PS_USHORT_4_4_4_4 && _newPixelFormat & PF_RGBA)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						newData[((updatedWidth * row + col) * 2) + 0] = (pixelArray[((pixelArrayRowLen * row + col) * 3) + 2] & 0xF0) |
							((pixelArray[((pixelArrayRowLen * row + col) * 3) + 1] & 0xF0) >> 4);

						newData[((updatedWidth * row + col) * 2) + 1] = pixelArray[((pixelArrayRowLen * row + col) * 3) + 0] | 0x0F;
					}
				}
			}

			//  BGR => RGBA (short)
			else if (_newPixelStructure & PS_USHORT_5_5_5_1 && _newPixelFormat & PF_RGBA)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						*reinterpret_cast<unsigned short*>(&(newData[((updatedWidth * row + col) * 2) + 0])) =
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 3) + 2] & 0xF8)) << 8) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 3) + 1] & 0xF8)) << 3) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 3) + 0] & 0xF8)) >> 2) |
							0x01;
					}
				}
			}

			//  BGR => RGB (short)
			else if (_newPixelStructure & PS_USHORT_5_6_5 && _newPixelFormat & PF_RGB)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						*reinterpret_cast<unsigned short*>(&(newData[((updatedWidth * row + col) * 2) + 0])) =
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 3) + 2] & 0xF8)) << 8) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 3) + 1] & 0xFC)) << 3) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 3) + 0] & 0xF8)) >> 3);
					}
				}
			}
		}
		//  from ABGR_8_8_8_8
		else if (fileHeader.bitsPerPixel == 32)
		{
			newData.resize(updatedWidth * updatedHeight * newPixelSize);

			//  ABGR => RGB
			if (_newPixelStructure & PS_BYTE_PER_COMPONENT && _newPixelFormat & PF_RGB)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						newData[((updatedWidth * row + col) * 3) + 0] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 3];
						newData[((updatedWidth * row + col) * 3) + 1] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 2];
						newData[((updatedWidth * row + col) * 3) + 2] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 1];
					}
				}
			}

			//  ABGR => RGBA
			else if (_newPixelStructure & PS_BYTE_PER_COMPONENT && _newPixelFormat & PF_RGBA)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						newData[((updatedWidth * row + col) * 4) + 0] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 3];
						newData[((updatedWidth * row + col) * 4) + 1] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 2];
						newData[((updatedWidth * row + col) * 4) + 2] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 1];
						newData[((updatedWidth * row + col) * 4) + 3] = pixelArray[((pixelArrayRowLen * row + col) * 4) + 0];
					}
				}
			}

			//  ABGR => RGBA (short)
			else if (_newPixelStructure & PS_USHORT_4_4_4_4 && _newPixelFormat & PF_RGBA)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						newData[((updatedWidth * row + col) * 2) + 0] = (pixelArray[((pixelArrayRowLen * row + col) * 4) + 3] & 0xF0) |
							((pixelArray[((pixelArrayRowLen * row + col) * 4) + 2] & 0xF0) >> 4);

						newData[((updatedWidth * row + col) * 2) + 1] = (pixelArray[((pixelArrayRowLen * row + col) * 4) + 1] & 0xF0) |
							((pixelArray[((pixelArrayRowLen * row + col) * 4) + 0] & 0xF0) >> 4);
					}
				}
			}

			//  ABGR => RGBA (short)
			else if (_newPixelStructure & PS_USHORT_5_5_5_1 && _newPixelFormat & PF_RGBA)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						*reinterpret_cast<unsigned short*>(&(newData[((updatedWidth * row + col) * 2) + 0])) =
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 3] & 0xF8)) << 8) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 2] & 0xF8)) << 3) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 1] & 0xF8)) >> 2) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 0] & 0x80)) >> 7);
					}
				}
			}

			//  ABGR => RGB (short)
			else if (_newPixelStructure & PS_USHORT_5_6_5 && _newPixelFormat & PF_RGB)
			{
				for (unsigned long int row = 0; row < fileHeader.imageHeight; ++row)
				{
					for (unsigned long int col = 0; col < fileHeader.imageWidth; ++col)
					{
						*reinterpret_cast<unsigned short*>(&(newData[((updatedWidth * row + col) * 2) + 0])) =
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 3] & 0xF8)) << 8) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 2] & 0xFC)) << 3) |
							(((unsigned short)(pixelArray[((pixelArrayRowLen * row + col) * 4) + 1] & 0xF8)) >> 3);
					}
				}
			}
		}
		//  from BGR_8
		//else if (fileHeader.bitsPerPixel == 8)
		//{}

		//  from BGR_4
		//else if (fileHeader.bitsPerPixel == 4)
		//{}

		//  from monohrome_1
		//else if (fileHeader.bitsPerPixel == 1)
		//{}


		// conversion completed, update image state

		width = updatedWidth;
		height = updatedHeight;

		format = updatedFormat;
		pixelFormat = updatedPixelFormat;
		pixelStructure = updatedPixelStructure;

		newData.swap(data);
	};

	void Image::convertRAWToBMP(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		throw std::logic_error("unimplemented");
	};


	void Image::convertJPGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2)
	{
		throw std::logic_error("unimplemented");
	};

	void Image::convertRAWToJPG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		throw std::logic_error("unimplemented");
	};


	void Image::convertPNGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2)
	{
		throw std::logic_error("unimplemented");
	};

	void Image::convertRAWToPNG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		throw std::logic_error("unimplemented");
	};



	void Image::read(DataStreamPtr _stream)
	{
		//make sure to reset position
		_stream->seekPosition(0);

		long int size = _stream->getSize();
		data.resize(size);
		data.shrink_to_fit();

		_stream->readData(&data[0]);

		width = 0;
		height = 0;

		format = getFormat(data);
		pixelFormat = PF_UNDEFINED;
		pixelStructure = PS_UNDEFINED;
	};


	void Image::write(DataStreamPtr _stream)
	{
		throw std::logic_error("unimplemented");
	};



	void Image::convert(IMAGE_FORMAT _newFormat, PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure)
	{
		assert(format != IF_UNDEFINED && "cannot convert uninitialised image");


		// convert to raw from other formats
		if (_newFormat == IF_RAW)
		{
			switch (format)
			{
			case IF_BMP:
				convertBMPToRAW(_newPixelFormat, _newPixelStructure, true);
				break;

			case IF_JPG:
				convertJPGToRAW(_newPixelFormat, _newPixelStructure, true);
				break;

			case IF_PNG:
				convertPNGToRAW(_newPixelFormat, _newPixelStructure, true);
				break;
			}

			return;
		}


		// if converting from one format to other change first to raw without changing color depth
		if (format != IF_RAW)
		{
			switch (format)
			{
			case IF_BMP:
				convertBMPToRAW(PF_UNDEFINED, PS_UNDEFINED, false);
				break;

			case IF_JPG:
				convertJPGToRAW(PF_UNDEFINED, PS_UNDEFINED, false);
				break;

			case IF_PNG:
				convertPNGToRAW(PF_UNDEFINED, PS_UNDEFINED, false);
				break;
			}
		}


		// convert to requested format
		switch (_newFormat)
		{
		case IF_BMP:
			convertRAWToBMP(_newPixelFormat, _newPixelStructure);
			break;

		case IF_JPG:
			convertRAWToJPG(_newPixelFormat, _newPixelStructure);
			break;

		case IF_PNG:
			convertRAWToPNG(_newPixelFormat, _newPixelStructure);
			break;
		}


		return;
	};



}
