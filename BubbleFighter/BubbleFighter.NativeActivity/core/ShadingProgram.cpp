#include "ShadingProgram.h"



namespace core
{

	ShadingProgram::ShadingProgram(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager), 
		id(0)
	{};

	ShadingProgram::~ShadingProgram()
	{};

	void ShadingProgram::loadImp()
	{
		uniformMap.clear();

		if (!verterShader.shader)
			verterShader.shader = ShaderManager::getSingleton().getByName(verterShader.name, group);

		if (!fragmentShader.shader)
			fragmentShader.shader = ShaderManager::getSingleton().getByName(fragmentShader.name, group);

		verterShader.shader->load();
		fragmentShader.shader->load();

		GLint linkStatus;

		try
		{
			GLuint tempId = 0;
			GL_ERROR_CHECK(tempId = glCreateProgram());
			id = tempId;

			GL_ERROR_CHECK(glAttachShader(id, verterShader.shader->getId()));
			GL_ERROR_CHECK(glAttachShader(id, fragmentShader.shader->getId()));

			GL_ERROR_CHECK(glLinkProgram(id));

			GL_ERROR_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
			if (linkStatus == GL_FALSE)
				throw std::runtime_error("glLinkProgram function failed");


			for (unsigned int index = 0; index < vertexAttribNames.size(); ++index)
			{
				GLint attribId(0);
				GL_ERROR_CHECK(attribId = glGetAttribLocation(id, vertexAttribNames[index]));

				if (attribId >= 0)
				{
					vertexAttribs[index].id = attribId;
					GL_ERROR_CHECK(glGetActiveAttrib(id, attribId, 0, nullptr, &(vertexAttribs[index].size), &(vertexAttribs[index].type), nullptr));
				}
			}

			GLint uniformCount(0), uniformNameLen(0);
			GL_ERROR_CHECK(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount));

			GL_ERROR_CHECK(glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLen));
			std::vector<char> uniformNameContainer(uniformNameLen);

			std::string uniformName;
			GLint uniformId;
			GLint uniformSize;
			GLenum uniformType;


			for (GLuint index; index < uniformCount; ++index)
			{
				GL_ERROR_CHECK(glGetActiveUniform(id, index, uniformNameContainer.size(), nullptr, &uniformSize, &uniformType, uniformNameContainer.data()));
				uniformName = std::string(uniformNameContainer.data());
				GL_ERROR_CHECK(uniformId = glGetUniformLocation(id, uniformName.c_str()));

				uniformMap.emplace(uniformName, Uniform(uniformName, uniformId, uniformSize, uniformType));
			}

		}
		catch (const std::exception &e)
		{
			Logger::getSingleton().write(e.what(), LL_ERROR);

			if (linkStatus == GL_FALSE)
			{
				GLint logLen;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLen);
				glGetError();

				std::string infoLog(logLen, '\0');
				glGetProgramInfoLog(id, logLen, nullptr, (&infoLog[0]));
				glGetError();

				Logger::getSingleton().write(infoLog, LL_DEBUG);
			}

			if (id)
			{
				glDeleteProgram(id);
				id = (GLuint)0;
				glGetError();
			}

			vertexAttribs.fill(Attrib());
			uniformMap.clear();

			throw;
		}

	};

	void ShadingProgram::unloadImp()
	{
		if (id)
		{
			glDeleteProgram(id);
			id = (GLuint)0;
			glGetError();
		}

		verterShader.shader.reset();
		fragmentShader.shader.reset();

		vertexAttribs.fill(Attrib());
		uniformMap.clear();
	};

	unsigned int ShadingProgram::sizeCalcImpl()
	{
		unsigned int s = sizeof(id);

		s += sizeof(verterShader);
		s += sizeof(fragmentShader);

		s += sizeof(id);

		s += sizeof(vertexAttribs);
		s += vertexAttribs.size() * sizeof(Attrib);

		//typedef std::map<std::string, Uniform> UniformNamedMap;
		s += sizeof(uniformMap);
		s += uniformMap.size();

		return s;
	};


	void ShadingProgram::setShader(std::string _shaderName)
	{
		ShaderPtr shader = ShaderManager::getSingleton().getByName(_shaderName);

		switch (shader->getType())
		{
		case ST_VERTEX:
			verterShader.name = _shaderName;
			verterShader.shader = shader;

		case ST_FRAGMENT:
			fragmentShader.name = _shaderName;
			fragmentShader.shader = shader;

		default:
			break;
		}
	};

}
