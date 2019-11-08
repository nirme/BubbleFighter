#include "Shader.h"

namespace core
{

	Shader::Shader(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager),
		id((GLuint)0),
		type((GLenum)0)
	{};

	Shader::~Shader()
	{};

	SHADER_TYPE Shader::getType()
	{
		switch (type)
		{
		case GL_VERTEX_SHADER:
			return ST_VERTEX;

		case GL_FRAGMENT_SHADER:
			return ST_FRAGMENT;

		default:
			return ST_UNKNOWN;
		}
	};


	void Shader::setType(SHADER_TYPE _type)
	{
		switch (_type)
		{
		case ST_VERTEX:
			type = GL_VERTEX_SHADER;
			break;

		case ST_FRAGMENT:
			type = GL_FRAGMENT_SHADER;
			break;

		default:
			type = (GLenum)0;
		}
	};


	void Shader::loadImp()
	{
		DataStreamPtr data = manager->openResource(this);

		std::vector<char> shaderData(data->getSize());
		data->readData(shaderData.data());

		GLint compileStatus;
		try
		{
			GLuint tempId = 0;
			GL_ERROR_CHECK(tempId = glCreateShader(type));
			id = tempId;

			GLint sourceLength = shaderData.size();
			const char *sourceText = shaderData.data();
			GL_ERROR_CHECK(glShaderSource(id, 1, &sourceText, &sourceLength));

			GL_ERROR_CHECK(glCompileShader(id));

			GL_ERROR_CHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
			if (compileStatus == GL_FALSE)
				throw std::runtime_error("glCompileShader function failed");
		}
		catch (const std::exception &e)
		{
			Logger::getSingleton().write(e.what(), LL_ERROR);

			if (compileStatus == GL_FALSE)
			{
				GLint logLen;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
				glGetError();

				std::string infoLog(logLen, '\0');
				glGetShaderInfoLog(id, logLen, nullptr, (&infoLog[0]));
				glGetError();

				Logger::getSingleton().write(infoLog, LL_DEBUG);
			}

			if (id)
			{
				glDeleteShader(id);
				id = (GLuint)0;
				glGetError();
			}

			throw;
		}
	};

	void Shader::unloadImp()
	{
		glDeleteShader(id);
		id = (GLuint)0;
		glGetError();
	};


	unsigned int Shader::sizeCalcImpl()
	{
		unsigned int s = sizeof(id);
		s += sizeof(type);

		if (state == RS_LOADED)
		{
			//GLint infoLen = 0;
			//glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);
			//s += infoLen;
			//glGetError();

			GLint sourceLen = 0;
			glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &sourceLen);
			s += sourceLen;
			glGetError();
		}

		return s;
	};


}