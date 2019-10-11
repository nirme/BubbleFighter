#include "Texture.h"

namespace core
{

	Texture::Texture(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager),
		id((GLuint)-1),
		filter(GL_NEAREST),
		loadedImage(nullptr)
	{};

	Texture::~Texture()
	{};


	void Texture::setType(TEXTURE_TYPE _type)
	{
		switch (_type)
		{
		case TT_TEXTURE_2D:
		default:
			type = GL_TEXTURE_2D;
		}
	};


	void Texture::setFilter(TEXTURE_FILTER _filter)
	{
		switch (_filter)
		{
		case TF_LINEAR:
			filter = GL_LINEAR;
			break;

		case TF_NEAREST:
		default:
			filter = GL_NEAREST;
		}
	};


	void Texture::loadImp()
	{
		DataStreamPtr data = manager->openResource(this);
		ImagePtr texImage(nullptr);

		texImage->read(data);
		texImage->convert(IF_RAW, PF_UNDEFINED, PS_BYTE_PER_COMPONENT);

		width = texImage->getWidth();
		height = texImage->getHeight();
		bitDepth = texImage->getBitDepth();

		texelFormat = texImage->getHWTexelFormat();
		texelStruct = texImage->getHWTexelStructure();


		id = (GLuint)-1;

		try
		{
			GL_ERROR_CHECK(glGenTextures(1, &id));

			GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, id));

			GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
			GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));

			GL_ERROR_CHECK(glTexImage2D(GL_TEXTURE_2D,
				0,
				texelFormat,
				width,
				height,
				0,
				texelFormat,
				texelStruct,
				loadedImage->getDataPtr()));

			GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		}
		catch (std::exception& e)
		{
			if (id != (GLuint)-1)
			{
				glDeleteTextures(1, &id);
				id = (GLuint)-1;
				glGetError();
			}

			Logger::getSingleton().write(e.what(), LL_ERROR);
			throw;
		}
	};


	void Texture::unloadImp()
	{
		ImagePtr texImage(nullptr);
		std::swap(loadedImage, texImage);

		glDeleteTextures(1, &id);
		id = (GLuint)-1;
	};

	unsigned int Texture::sizeCalcImpl()
	{
		unsigned int s = sizeof(id);
		s += sizeof(filter);

		s += sizeof(width);
		s += sizeof(height);
		s += sizeof(bitDepth);

		s += sizeof(texelFormat);
		s += sizeof(texelStruct);

		s += sizeof(loadedImage);
		if (loadedImage)
			s += loadedImage->getSize();
		else if (state == RS_LOADED)
			s += width * height * bitDepth / 8;

		return s;
	};


}