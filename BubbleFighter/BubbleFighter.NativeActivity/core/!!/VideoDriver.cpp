#include "VideoDriver.h"

namespace Graphic
{


	void VideoDriver::setupBillboardsBuffers()
	{
		if (billboardBuffersAlocated)
			throw ApplicationException("Billboard buffers are already alocated");

		GLenum err = GL_NO_ERROR;
		GLuint buffers[2];
		glGenBuffers(2, buffers);
		assert(glGetError() == GL_NO_ERROR);

		if ((err = glGetError()) != GL_NO_ERROR)
			throw GlesException(err);


		index2DBufferId = buffers[0];
		vertex2DBufferId = buffers[1];

		billboardDrawMode = GL_TRIANGLES;
		bilboardIndexType = GL_UNSIGNED_BYTE;

		std::vector<GLubyte> indices(maxBillboardsPerDrawCall * 6);

		for (unsigned int i = 0; i < maxBillboardsPerDrawCall; ++i)
		{
			indices[i * 6 + 0] = (GLubyte)i * 4 + 0;
			indices[i * 6 + 1] = (GLubyte)i * 4 + 1;
			indices[i * 6 + 2] = (GLubyte)i * 4 + 3;
			indices[i * 6 + 3] = (GLubyte)i * 4 + 0;
			indices[i * 6 + 4] = (GLubyte)i * 4 + 3;
			indices[i * 6 + 5] = (GLubyte)i * 4 + 2;
		}

		// setup indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index2DBufferId);
		assert(glGetError() == GL_NO_ERROR);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);

		// reserve vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertex2DBufferId);
		assert(glGetError() == GL_NO_ERROR);

		vertex2DBufferData.resize(maxBillboardsPerDrawCall * 4 * 3 * 2); //4 verts per billboard, assuming that most billboards will use up to 2 textures (3 pairs of 2 floats)
		glBufferData(GL_ARRAY_BUFFER, vertex2DBufferData.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);

		if ((err = glGetError()) != GL_NO_ERROR)
		{
			//in case of errors delete buffers
			glDeleteBuffers(2, buffers);

			//clear error queue
			while (glGetError() != GL_NO_ERROR);

			// throw the error
			throw GlesException(err);
		}

		billboardBuffersAlocated = true;
	};


	void VideoDriver::deleteBillboardsBuffers()
	{
		if (billboardBuffersAlocated)
			return;

		GLuint buffers[2] = { index2DBufferId, vertex2DBufferId };
		// delete buffers
		glDeleteBuffers(2, buffers);

		index2DBufferId = -1;
		vertex2DBufferId = -1;

		// remove cashe vec data
		vertex2DBufferData.erase(vertex2DBufferData.begin(), vertex2DBufferData.end());

		//clear error queue
		while (glGetError() != GL_NO_ERROR);

		billboardBuffersAlocated = false;
	};











	GLuint VideoDriver::loadTexture2D(
		GLsizei _width,
		GLsizei _height,
		GLenum _format,
		GLenum _dataType,
		const std::vector<std::pair<GLenum, boost::variant<GLint, GLfloat>>>& _parameters,
		const std::vector<unsigned char>& _data,
		bool _isCompressed = false)
	{
		GLuint textureId;
		GLenum err = GL_NO_ERROR;

		//activate last texture unit (8 units min are guaranted), we don't want to override anything in use
		glActiveTexture(GL_TEXTURE7);
		assert(glGetError() == GL_NO_ERROR);

		//generate tex id
		glGenTextures(1, &textureId);
		assert(glGetError() == GL_NO_ERROR);

		if ((err = glGetError()) != GL_NO_ERROR)
			throw GlesException(err);


		//bind the texture to set it up
		glBindTexture(GL_TEXTURE_2D, textureId);
		assert(glGetError() == GL_NO_ERROR);

		//set each of the params provided
		for (int i = 0; i < _parameters.size(); ++i)
		{
			switch (_parameters[i].second.which())
			{
			case 0:
				glTexParameteri(GL_TEXTURE_2D, _parameters[i].first, boost::get<GLint>(_parameters[i].second));
				break;

			case 1:
				glTexParameterf(GL_TEXTURE_2D, _parameters[i].first, boost::get<GLfloat>(_parameters[i].second));
				break;
			}
			assert(glGetError() == GL_NO_ERROR);
		}

		//setup tex size and format, and data if provided
		if (!_isCompressed)
			glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, _dataType, _data.size() > 0 ? _data.data() : NULL);
		else
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _data.size(), _data.data());

		assert(glGetError() == GL_NO_ERROR);


		//unbind the tex id
		glBindTexture(GL_TEXTURE_2D, 0);
		assert(glGetError() == GL_NO_ERROR);

		if ((err = glGetError()) != GL_NO_ERROR)
		{
			//delete texture for good measure
			glDeleteTextures(1, &textureId);

			//clear error queue
			while (glGetError() != GL_NO_ERROR);

			throw GlesException(err);
		}

		auto result = allocatedTextureIds.insert(textureId);
		assert(result.first != allocatedTextureIds.end());

		return textureId;
	};


	void VideoDriver::unloadTexture(GLuint textureId)
	{
		//delete texture
		glDeleteTextures(1, &textureId);
		assert(glGetError() == GL_NO_ERROR);

		//clear error queue
		while (glGetError() != GL_NO_ERROR);

		//remove id from list
		allocatedTextureIds.erase(textureId);
	};


	GLuint VideoDriver::loadShader(GLenum shaderType, const std::string& shaderSource)
	{
		GLuint shaderId;
		GLenum err = GL_NO_ERROR;

		shaderId = glCreateShader(shaderType);
		assert(glGetError() == GL_NO_ERROR);

		if ((err = glGetError()) != GL_NO_ERROR)
			throw GlesException(err);


		//set values acceptable in glShaderSource
		const GLint sourceLength(shaderSource.length());
		const GLchar* sourcePtr((const GLchar*)shaderSource.c_str());

		glShaderSource(shaderId, 1, &sourcePtr, &sourceLength);
		assert(glGetError() == GL_NO_ERROR);


		if ((err = glGetError()) != GL_NO_ERROR)
		{
			//delete shader for good measure
			glDeleteShader(shaderId);

			//clear error queue
			while (glGetError() != GL_NO_ERROR);
			throw GlesException(err);
		}


		glCompileShader(shaderId);

		GLint param;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &param);

		if (param == GL_FALSE)
		{
			//get compile info
			GLint infoLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);

			std::string infoLog((std::size_t)infoLength, '\0');
			glGetShaderInfoLog(shaderId, infoLength, NULL, &(infoLog[0]));

			//delete shader for good measure
			glDeleteShader(shaderId);

			throw GlesCompileError(infoLog);
		}


		if ((err = glGetError()) != GL_NO_ERROR)
		{
			//delete shader for good measure
			glDeleteShader(shaderId);

			//clear error queue
			while (glGetError() != GL_NO_ERROR);
			throw GlesException(err);
		}



		auto result = allocatedShaderIds.insert(shaderId);
		assert(result.first != allocatedTextureIds.end());

		return shaderId;
	};


	void VideoDriver::unloadShader(GLuint shaderId)
	{
		//delete shader
		glDeleteShader(shaderId);
		assert(glGetError() == GL_NO_ERROR);

		//clear error queue
		while (glGetError() != GL_NO_ERROR);

		//remove id from list
		allocatedShaderIds.erase(shaderId);
	};


	GLuint VideoDriver::loadProgram(std::vector<GLuint> _shaders)
	{
		GLuint programId;
		GLenum err = GL_NO_ERROR;

		programId = glCreateProgram();
		assert(glGetError() == GL_NO_ERROR);

		if ((err = glGetError()) != GL_NO_ERROR)
			throw GlesException(err);


		for (int i = 0; i < defaultAttribLocations.size(); ++i)
		{
			glBindAttribLocation(programId, defaultAttribLocations[i].first, (const GLchar*)defaultAttribLocations[i].second.c_str());
			assert(glGetError() == GL_NO_ERROR);
		}

		for (int i = 0; i < _shaders.size(); ++i)
		{
			glAttachShader(programId, _shaders[i]);
			assert(glGetError() == GL_NO_ERROR);
		}

		glLinkProgram(programId);
		assert(glGetError() == GL_NO_ERROR);

		GLint linkStatus;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
		assert(glGetError() == GL_NO_ERROR);


		if ((err = glGetError()) != GL_NO_ERROR || linkStatus != GL_TRUE)
		{
			//get linking info when no error occured
			if (err == GL_NO_ERROR)
			{
				GLint infoLength;
				glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLength);

				std::string infoLog((std::size_t)infoLength, '\0');
				glGetProgramInfoLog(programId, infoLength, NULL, &(infoLog[0]));

				//delete program
				glDeleteProgram(programId);
				//clear error queue
				while (glGetError() != GL_NO_ERROR);

				throw GlesLinkingError(infoLog);
			}

			//delete program
			glDeleteProgram(programId);
			//clear error queue
			while (glGetError() != GL_NO_ERROR);

			throw GlesException(err);
		}

		auto result = allocatedProgramIds.insert(programId);
		assert(result.first != allocatedProgramIds.end());

		return programId;
	};

	void VideoDriver::unloadProgram(GLuint programId)
	{
		//delete program
		glDeleteProgram(programId);
		assert(glGetError() == GL_NO_ERROR);

		//clear error queue
		while (glGetError() != GL_NO_ERROR);

		//remove id from list
		allocatedProgramIds.erase(programId);
	};






	
	GlesException::GlesException(GLenum _error) 
		: error(_error)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			errorMsg = "An unacceptable value is specified for an enumerated argument.The offending command is ignored and has no other side effect than to set the error flag.";
			break;

		case GL_INVALID_VALUE:
			errorMsg = "A numeric argument is out of range.The offending command is ignored and has no other side effect than to set the error flag.";
			break;

		case GL_INVALID_OPERATION:
			errorMsg = "The specified operation is not allowed in the current state.The offending command is ignored and has no other side effect than to set the error flag.";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorMsg = "The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete(i.e.the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE).The offending command is ignored and has no other side effect than to set the error flag.";
			break;

		case GL_OUT_OF_MEMORY:
			errorMsg = "There is not enough memory left to execute the command.The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
			break;

		default:
			errorMsg = "Unknown openGL error occured.";
			break;
		}
	}

	const char* GlesException::what() const noexcept
	{
		return errorMsg;
	};




	GlesExceptionWithInfo::GlesExceptionWithInfo(std::string& _infoLog) 
		: GlesException(-1)
	{
		errorMsg = "Shader compilation error.";
		infoLog.swap(_infoLog);
	};

	const std::string& GlesExceptionWithInfo::getInfoLog() const
	{
		return infoLog;
	};


	GlesCompileError::GlesCompileError(std::string& _infoLog) 
		: GlesExceptionWithInfo(_infoLog)
	{};


	GlesLinkingError::GlesLinkingError(std::string& _infoLog) 
		: GlesExceptionWithInfo(_infoLog)
	{};




}
