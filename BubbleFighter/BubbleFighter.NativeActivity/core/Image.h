#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "DataStream.h"

#define	PO2(val)	(((((((--val) | val >> 16) | val >> 8) | val >> 4) | val >> 2) | val >> 1) + 1);



namespace core
{

	class Image
	{
	public:

		enum IMAGE_FORMAT
		{
			IF_UNDEFINED = 0x00,
			IF_RAW = 0x01,
			IF_BMP = 0x02,
			IF_JPG = 0x04,
			IF_PNG = 0x08,
		};

		enum PIXEL_FORMAT
		{
			PF_UNDEFINED = 0x00,
			PF_RGB = 0x01,
			PF_RGBA = 0x02,
			//PF_ALPHA = 0x04,
			//PF_LUMINANCE = 0x08,
			//PF_LUMINANCE_ALPHA = 0x10,
		};

		enum PIXEL_STRUCTURE
		{
			PS_UNDEFINED = 0x00,
			PS_BYTE_PER_COMPONENT = 0x01,
			PS_USHORT_4_4_4_4 = 0x02, 
			PS_USHORT_5_5_5_1 = 0x04, 
			PS_USHORT_5_6_5 = 0x08,
		};


	protected:

		unsigned int width;
		unsigned int height;

		IMAGE_FORMAT format;
		PIXEL_FORMAT pixelFormat;
		PIXEL_STRUCTURE pixelStructure;

		std::vector<unsigned char> data;


		static IMAGE_FORMAT getFormat(const std::vector<unsigned char> _header);


		void convertBMPToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2 = true);
		void convertRAWToBMP(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure);

		void convertJPGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2 = true);
		void convertRAWToJPG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure);

		void convertPNGToRAW(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure, bool _roundSizeToPow2 = true);
		void convertRAWToPNG(PIXEL_FORMAT _newPixelFormat, PIXEL_STRUCTURE _newPixelStructure);


	public:

		Image();

		void read(DataStreamPtr _stream);
		void write(DataStreamPtr _stream);

		void convert(IMAGE_FORMAT _newFormat = IF_RAW, PIXEL_FORMAT _newPixelFormat = PF_UNDEFINED, PIXEL_STRUCTURE _newPixelStructure = PS_UNDEFINED);


	};
}