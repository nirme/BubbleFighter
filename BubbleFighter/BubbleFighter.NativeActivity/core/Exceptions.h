#pragma once

#include <exception>
#include <stdexcept>
#include <string>


#define GL_ERROR_CHECK(glFunc) \
	{ \
		glFunc; \
		GLenum err = glGetError(); \
		switch (err) \
		{ \
			case GL_INVALID_ENUM: \
				throw std::runtime_error("#glFunc function failed with error GL_INVALID_ENUM"); \
			case GL_INVALID_VALUE: \
				throw std::runtime_error("#glFunc function failed with error GL_INVALID_VALUE"); \
			case GL_INVALID_OPERATION : \
				throw std::runtime_error("#glFunc function failed with error GL_INVALID_OPERATION"); \
			case GL_INVALID_FRAMEBUFFER_OPERATION : \
				throw std::runtime_error("#glFunc function failed with error GL_INVALID_FRAMEBUFFER_OPERATION"); \
			case GL_OUT_OF_MEMORY : \
				throw std::runtime_error("#glFunc function failed with error GL_OUT_OF_MEMORY"); \
		} \
	}\

	



/*
class GlException : public std::exception
{
protected:
	const char errorMsg[4096];

public:
	GlException(GLenum _error) :
		error(_error)
	{
		switch (_error)
		{
			errorMsg = "test";
			GL_INVALID_ENUM

				GL_INVALID_VALUE

				GL_INVALID_OPERATION

				GL_INVALID_FRAMEBUFFER_OPERATION
				The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete(i.e.the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE).The offending command is ignored and has no other side effect than to set the error flag.

				GL_OUT_OF_MEMORY

		}
		errorMsg = "test";
	};
	virtual const char* what() const noexcept;
};


*/



/*

class NotImplementedException : public std::logic_error
{
public:
	NotImplementedException() : std::logic_error("Function not yet implemented.") {};
};


namespace Graphic
{

	class ApplicationException : public std::exception
	{
	protected:
		std::string errorMsg;

	public:
		ApplicationException(std::string _errorMsg)
			: errorMsg(_errorMsg)
		{};

		virtual const char* what() const noexcept
		{
			return errorMsg.c_str();
		};
	};

	class AndroidException : public ApplicationException
	{
	protected:
		int32_t error;

	public:
		AndroidException(int32_t _error, std::string _functionName)
			: ApplicationException("Android function '" + _functionName + "' failed with code: "), error(_error)
		{
			//errorMsg += std::to_string(_error);

			char num[12];
			sprintf(num, "%#08X", error);
			errorMsg += num;
		};
	};



	class EGLException : public std::exception
	{
	protected:
		EGLint error;
		const char* errorMsg;

	public:
		EGLException(EGLint _error) {};
		virtual const char* what() const noexcept {};
	};



	class GlesException : public std::exception
	{
	protected:
		GLenum error;
		const char* errorMsg;

	public:
		GlesException(GLenum _error);
		virtual const char* what() const noexcept;
	};

	class GlesExceptionWithInfo : public GlesException
	{
	protected:
		std::string infoLog;

	public:
		GlesExceptionWithInfo(std::string& _infoLog);
		const std::string& getInfoLog() const;
	};

	class GlesCompileError : public GlesExceptionWithInfo
	{
	public:
		GlesCompileError(std::string& _infoLog);
	};

	class GlesLinkingError : public GlesExceptionWithInfo
	{
	public:
		GlesLinkingError(std::string& _infoLog);
	};

}
*/
