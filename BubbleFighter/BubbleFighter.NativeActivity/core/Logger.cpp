#include <android/log.h>
#include "Logger.h"



template<>
core::Logger* Singleton<core::Logger>::impl = nullptr;


namespace core
{

	Logger& Logger::getSingleton()
	{
		return *impl;
	};

	Logger* Logger::getSingletonPtr()
	{
		return impl;
	};


	Logger::Logger() :
		logLevel(LL_INFO)
	{};

	Logger::~Logger()
	{};


	Logger& Logger::write(const char* _text, LOG_LEVEL _logLevel)
	{
		if (_logLevel <= logLevel)
		{
			__android_log_write(
				_logLevel < priorityMap.size() ? priorityMap.back() : priorityMap[_logLevel],
				"app", 
				_text
			);
		}

		return *this;
	};

	Logger& Logger::write(std::string _text, LOG_LEVEL _logLevel)
	{
		return write(_text.c_str(),_logLevel);
	};

}