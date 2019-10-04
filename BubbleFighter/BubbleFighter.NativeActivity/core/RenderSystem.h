#pragma once

#include <list>
#include <utility>

#include "SingletonTemplate.h"
#include "Exceptions.h"


typedef std::list<std::pair<GLenum, GLint>> ParamList;


namespace core 
{

	class RenderSystem : public Singleton<RenderSystem>
	{


	public:

		enum GRAPHIC_CAPABILITY
		{
			TEXTURE_BGRA_8888 = 0x0001, //EXT_texture_format_BGRA8888
		};




		bool hasCapabilty(GRAPHIC_CAPABILITY gc_capability)
		{
			return true;
		};


	};

}