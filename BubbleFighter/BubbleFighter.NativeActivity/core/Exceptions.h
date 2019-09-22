#pragma once

#include <exception>
#include <stdexcept>
#include <string>


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
