#pragma once

#include <Simple_Logger/Simple_Logger.h>

constexpr const char* getFileName(const char* path)
{
	const char* file = path;
	while (*path)
	{
		if (*path == '/' || *path == '\\')
		{
			file = path + 1;
		}
		++path;
	}
	return file;
}

#define __FILENAME__ getFileName(__FILE__)

namespace W_Engine
{
	class Log
	{
	public:
		inline static Simple_Logger::Logger& GetLogger()
		{
			static Log log;
			if (!log.m_initialized)
			{
				log.m_logger = Simple_Logger::Logger{};
				log.m_logger.init();
				log.m_initialized = true;
			}
			return log.m_logger;
		}
	private:
		bool m_initialized = false;
		Simple_Logger::Logger m_logger = Simple_Logger::Logger{};
	};
}

#define LOG_ERROR(...) W_Engine::Log::GetLogger().error(__FILENAME__, __LINE__, __VA_ARGS__)

#ifdef RELEASE
#define LOG_DEBUG(...) ((void)0)
#define LOG_WARNING(...) ((void)0)
#else
#define LOG_DEBUG(...) W_Engine::Log::GetLogger().debug(__FILENAME__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) W_Engine::Log::GetLogger().warning(__FILENAME__, __LINE__, __VA_ARGS__)
#endif