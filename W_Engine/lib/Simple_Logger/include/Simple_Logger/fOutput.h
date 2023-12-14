#pragma once

#include <cstdio>
#include <string>

namespace Simple_Logger
{
	namespace fOutput
	{
		template <typename T>
		inline T adjust(T value) noexcept;

		template <typename T>
		inline const T* adjust(const std::basic_string<T>& value) noexcept;

		template <typename ... Args>
		inline int getString(char* const buffer, const size_t bufferCount, const char* const format, const Args& ... args) noexcept;

		template <typename ... Args>
		inline int getString(char* const buffer, const size_t bufferCount, const wchar_t* const format, const Args& ... args) noexcept;

		// =====================================
		// printing to the console =============
		// =====================================
		template <typename ... Args>
		inline void print(const char* const format, const Args& ... args) noexcept
		{
			std::printf(format, adjust(args) ...);
		}

		template <typename T, typename ... Args>
		inline void print(const std::basic_string<T>& format, const Args& ... args) noexcept
		{
			print(adjust(format), args ...);
		}

		inline void print(const char* const value) noexcept
		{
			print("%s", value);
		}

		inline void print(const wchar_t* const value) noexcept
		{
			print("%ls", value);
		}

		template <typename T>
		inline void print(const std::basic_string<T>& value) noexcept
		{
			print(value.c_str());
		}

		template <typename T>
		inline T adjust(T value) noexcept
		{
			return value;
		}

		template <typename T>
		inline const T* adjust(const std::basic_string<T>& value) noexcept
		{
			return value.c_str();
		}

		// =====================================
		// formatting a string =================
		// =====================================

		template <typename T, typename ... Args>
		inline void format(std::basic_string<T>& buffer, const T* const format, const Args& ... args)
		{
			const size_t size = getString(&buffer[0], buffer.size() + 1, format, args ...);
			// if the string didn't fit in the buffer
			if (size > buffer.size())
			{
				buffer.resize(size);
				getString(&buffer[0], buffer.size() + 1, format, args ...);
			} // if the buffer is larger than the string
			else if (size < buffer.size())
			{
				buffer.resize(size);
			}
			// if it fit perfectly the first time don't do anything
		}

		template <typename ... Args>
		inline int getString(char* const buffer, const size_t bufferCount, const char* const format, const Args& ... args) noexcept
		{
			return snprintf(buffer, bufferCount, format, adjust(args) ...);
		}

		template <typename ... Args>
		inline int getString(char* const buffer, const size_t bufferCount, const wchar_t* const format, const Args& ... args) noexcept
		{
			return swprintf(buffer, bufferCount, format, adjust(args) ...);
		}
	}
}