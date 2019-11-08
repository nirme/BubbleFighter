#pragma once

#include <android/log.h>
#include <string>
#include <array>

#include "SingletonTemplate.h"


namespace core
{

	enum LOG_LEVEL : unsigned char
	{
		LL_NONE = 0x00, //not used in write operation
		LL_CRITICAL = 0x01,
		LL_ERROR = 0x02,
		LL_WARNING = 0x03,
		LL_INFO = 0x04,
		LL_DEBUG = 0x05,
		LL_ALL = 0xFF, //not used in write operation

	};


	class Logger : public Singleton<Logger>
	{
	private:
		LOG_LEVEL logLevel;

		static constexpr std::array<android_LogPriority, 7> priorityMap{ 
			ANDROID_LOG_UNKNOWN, 
			ANDROID_LOG_FATAL,
			ANDROID_LOG_ERROR,
			ANDROID_LOG_WARN,
			ANDROID_LOG_INFO,
			ANDROID_LOG_DEBUG,
			ANDROID_LOG_VERBOSE,
		};


		inline android_LogPriority getAndroidLogLevel(LOG_LEVEL _level)
		{
			return _level < priorityMap.size() ? priorityMap.back() : priorityMap[_level];
		};

	public:

		Logger();
		virtual ~Logger();


		void setLevel(LOG_LEVEL _logLevel)
		{
			logLevel = _logLevel;
		};

		LOG_LEVEL getLevel()
		{
			return logLevel;;
		};


		static Logger& getSingleton();
		static Logger* getSingletonPtr();

		Logger& write(const char* _text, LOG_LEVEL _logLevel = LL_INFO);
		Logger& write(std::string _text, LOG_LEVEL _logLevel = LL_INFO);
	};
}