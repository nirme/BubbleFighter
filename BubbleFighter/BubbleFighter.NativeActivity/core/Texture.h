#pragma once

#include <memory>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"

#include "Resource.h"
#include "ResourceManager.h"
#include "DataStream.h"
#include "Image.h"



namespace core
{

	enum TEXTURE_TYPE
	{
		TT_TEXTURE_2D = 0x00,
	};


	enum TEXTURE_FILTER
	{
		TF_NEAREST = 0x00,
		TF_LINEAR = 0x01,
	};


	class Texture : public Resource
	{
	protected:

		GLuint id;
		GLint type;
		GLint filter;

		typedef std::unique_ptr<Image> ImagePtr;
		//ImagePtr loadedImage;

		unsigned int width;
		unsigned int height;
		unsigned int bitDepth;

		GLint texelFormat;
		GLint texelStruct;


	public:

		Texture(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager = nullptr);

		virtual ~Texture();

		virtual void setType(TEXTURE_TYPE _type);
		virtual void setFilter(TEXTURE_FILTER _filter);

		virtual void loadImp();
		virtual void unloadImp();

		virtual unsigned int sizeCalcImpl();

	};





	typedef std::shared_ptr<Texture> TexturePtr;

}
