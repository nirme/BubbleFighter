#pragma once

#include <memory>
#include <vector>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"

#include "Resource.h"
#include "ResourceManager.h"


namespace core
{
	enum SHADER_TYPE : unsigned short
	{
		ST_UNKNOWN = 0x00,
		ST_VERTEX = 0x01,
		ST_FRAGMENT = 0x02,
	};

	class Shader : public Resource
	{
	protected:

		GLuint id;
		GLenum type;

	public:

		Shader(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager = nullptr);

		~Shader();

		SHADER_TYPE getType();
		void setType(SHADER_TYPE _type);

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		inline GLuint getId() const { return id; };
	};

	typedef std::shared_ptr<Shader> ShaderPtr;

}