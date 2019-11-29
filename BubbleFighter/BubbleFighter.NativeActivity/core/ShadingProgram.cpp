#include "ShadingProgram.h"



namespace core
{

	ShadingProgram::ShadingProgram(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager) :
		Resource(_name, _handle, _group, _manager), 
		id(0)
	{};

	ShadingProgram::~ShadingProgram()
	{};

	void ShadingProgram::loadImp()
	{

		DataStreamPtr data = manager->openResource(getName());
		ScriptLoader &sloader = ScriptLoader::getSingleton();
		ScriptNodeListPtr spriteDataList = sloader.parse(data);

		for (auto it = spriteDataList->begin(); it != spriteDataList->end(); ++it)
		{
			std::string nodeType = (*it)->getName();
			if (nodeType.compare("shader") == 0)
			{

				SHADER_TYPE type = sloader.parseShaderType(*it);

				if (type == ST_UNKNOWN)
					continue;

				std::string shaderName = sloader.parseShaderName(*it);
				shaders[type] = ShaderManager::getSingleton().getByName(shaderName, getGroup());
				shaders[type]->load();
			}
		}
		

		paramsList = std::make_unique<ShadingProgramParams>();

		GLint linkStatus;

		try
		{
			if (!shaders[ST_VERTEX])
				throw std::runtime_error("shader of type ST_VERTEX not declared per program");

			if (!shaders[ST_FRAGMENT])
				throw std::runtime_error("shader of type ST_FRAGMENT not declared per program");


			GLuint tempId = 0;
			GL_ERROR_CHECK(tempId = glCreateProgram());
			id = tempId;

			GL_ERROR_CHECK(glAttachShader(id, shaders[ST_VERTEX]->getId()));
			GL_ERROR_CHECK(glAttachShader(id, shaders[ST_FRAGMENT]->getId()));

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

				paramsList->addUniformDefinition(uniformName, uniformId, uniformSize, uniformType);
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

			shaders[ST_VERTEX].reset();
			shaders[ST_FRAGMENT].reset();

			vertexAttribs.clear();
			vertexAttribs.shrink_to_fit();
			paramsList.reset();

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

		shaders[ST_VERTEX].reset();
		shaders[ST_FRAGMENT].reset();

		vertexAttribs.clear();
		vertexAttribs.shrink_to_fit();
		paramsList.reset();
	};

	unsigned int ShadingProgram::sizeCalcImpl()
	{
		unsigned int s = sizeof(id);

		s += sizeof(shaders[ST_VERTEX]);
		s += sizeof(shaders[ST_FRAGMENT]);

		s += sizeof(id);

		s += sizeof(vertexAttribs);
		s += vertexAttribs.size() * sizeof(Attrib);

		//typedef std::map<std::string, Uniform> UniformNamedMap;
		s += sizeof(paramsList);
		s += sizeof(ShadingProgramParams);

		return s;
	};

}
