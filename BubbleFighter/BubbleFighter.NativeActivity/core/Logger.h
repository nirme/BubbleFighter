#pragma once

#include <string>

#include "SingletonTemplate.h"


namespace core
{

	enum LOG_LEVEL
	{
		LL_NONE = 0x00,
		LL_CRITICAL = 0x01,
		LL_ERROR = 0x02,
		LL_WARNING = 0x04,
		LL_INFO = 0x08,
		LL_DEBUG = 0x0F,
		LL_ALL = 0xFF,

	};


	class Logger : public Singleton<Logger>
	{
	private:
		LOG_LEVEL logLevel;

	public:

		Logger& write(const char* _text, LOG_LEVEL _logLevel = LL_INFO)
		{
			if (_logLevel <= logLevel)
			{
				//log
			}

			return *this;
		};

		Logger& write(std::string _text, LOG_LEVEL _logLevel = LL_INFO)
		{
			return write(_text.c_str(),_logLevel);
		};
	};
}