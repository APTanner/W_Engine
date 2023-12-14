#pragma once

#include <Windows.h>
#include <string>

#include "fOutput.h"

namespace Simple_Logger
{
	class Logger
	{
	public:
		enum class LogLevel : int
		{
			debug,
			warning,
			error
		};

		void init()
		{
			setLevel(Logger::LogLevel::debug);
			m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		void setLevel(const LogLevel level)
		{
			m_level = level;
		}

		template<typename ... Args>
		void debug(const char* file, int line, const char* const format, const Args& ... args) const
		{
			log(file, line, LogLevel::debug, format, args ...);
		}

		template<typename ... Args>
		void debug(const char* file, int line, const wchar_t* const format, const Args& ... args) const
		{
			log(file, line, LogLevel::debug, format, args ...);
		}

		template<typename ... Args>
		void debug(const char* file, int line, const std::string& format, const Args& ... args) const
		{
			log(file, line, LogLevel::debug, format.c_str(), args ...);
		}

		template<typename ... Args>
		void warning(const char* file, int line, const char* const format, const Args& ... args) const
		{
			log(file, line, LogLevel::warning, format, args ...);
		}

		template<typename ... Args>
		void warning(const char* file, int line, const wchar_t* const format, const Args& ... args) const
		{
			log(file, line, LogLevel::warning, format, args ...);
		}

		template<typename ... Args>
		void warning(const char* file, int line, const std::string& format, const Args& ... args) const
		{
			log(file, line, LogLevel::warning, format.c_str(), args ...);
		}

		template<typename ... Args>
		void error(const char* file, int line, const char* const format, const Args& ... args) const
		{
			log(file, line, LogLevel::error, format, args ...);
		}

		template<typename ... Args>
		void error(const char* file, int line, const wchar_t* const format, const Args& ... args) const
		{
			log(file, line, LogLevel::error, format, args ...);
		}

		template<typename ... Args>
		void error(const char* file, int line, const std::string& format, const Args& ... args) const
		{
			log(file, line, LogLevel::error, format.c_str(), args ...);
		}
	private:
		LogLevel m_level;
		HANDLE m_hConsole;

		template<typename T>
		void logPrefix(std::basic_string<T>& buffer, const LogLevel level, const char* file, int line) const
		{
			// TODO other stuff here in the future (timing/file and location)
			fOutput::format(buffer, "[%s] [%s::%i] ", getLevelName(level), file, line);
		}

		const std::string getLevelName(const LogLevel level) const
		{
			switch (level)
			{
			case LogLevel::debug:   return "DEBUG";
			case LogLevel::warning: return "WARNING";
			case LogLevel::error:   return "ERROR";
			default:				return "UNKNOWN";
			}
		}

		void setConsoleColor(const LogLevel level) const
		{
			switch (level)
			{
			case LogLevel::debug:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case LogLevel::warning:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
				break;
			case LogLevel::error:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED);
				break;
			default:
				SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}

		void clearConsoleColor() const
		{
			SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		template<typename T, typename ... Args>
		void log(const char* file, int line, const LogLevel level, const T* format, const Args& ... args) const
		{
			if (m_level > level)
				return;
			std::basic_string<T> buffer;
			// append the stuff around the message
			logPrefix(buffer, level, file, line);
			buffer += format;
			buffer += '\n';
			setConsoleColor(level);
			fOutput::print(buffer, args ...);
			clearConsoleColor();
		}
	};
}

