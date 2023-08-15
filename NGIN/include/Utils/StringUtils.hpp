#pragma once

#include <HideWarnings/fmt.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <source_location>


namespace NGIN
{
	namespace Util
	{
		/**
		 * @brief Converts a string to the specified type T.
		 *
		 * This function attempts to convert a given string to type T using the extraction
		 * operator. If the conversion is unsuccessful, a runtime error is thrown.
		 *
		 * @tparam T The target type to which the string should be converted.
		 * @param s The string to convert.
		 * @return The string converted to type T.
		 * @throws std::runtime_error If the conversion fails.
		*/
		template <typename T>
		T FromString(const std::string& s, const std::source_location& source = std::source_location::current())
		{
			std::stringstream ss(s);
			T value;
			ss >> value;

			return value;
		}
		/**
		 * @brief Specialization of the FromString function for std::string.
		 *
		 * Since the target type is already a string, this function returns the input string directly.
		 *
		 * @param s The string to return.
		 * @return The input string.
		 */
		template <>
		inline std::string FromString<std::string>(const std::string& s, const std::source_location& source)
		{
			return s;
		}

		/**
		 * @brief Extracts the file name from a given full path string.
		 *
		 * This function looks for the last occurrence of both forward and backward slashes
		 * in the given path string, and returns the substring after the last slash.
		 * It is cross-platform and accounts for different directory separator conventions.
		 *
		 * @param fullPath The full path string from which the file name should be extracted.
		 * @return A pointer to the character where the file name starts within the fullPath.
		 */
		inline const char* ExtractFileName(const char* fullPath)
		{
			// Find the last backslash, diffrent OS can use diffrent slashes
			const char* fromSlash = strrchr(fullPath, '/');
			const char* fromBackslash = strrchr(fullPath, '\\');

			// Determine which one is the last separator in the path.
			const char* lastSeparator = (fromSlash > fromBackslash) ? fromSlash : fromBackslash;

			return (lastSeparator) ? lastSeparator + 1 : fullPath;
		}

		/**
		 * @brief Performs runtime formatting on a given message string using provided format arguments.
		 *
		 * This function allows for dynamically formatting a message string based on the given formatArgs tuple.
		 * The actual formatting is performed using the fmt library.
		 *
		 * @tparam Args The types of format arguments contained in the formatArgs tuple.
		 * @param message The message string to be formatted.
		 * @param formatArgs A tuple containing the arguments to be used for formatting the message.
		 * @return A formatted string based on the provided message and format arguments.
		 */
		template <typename... Args>
		std::string RuntimeFormat(std::string_view message, const std::tuple<Args...>& formatArgs)
		{
			std::string format;
			Util::Apply(formatArgs, [&](auto&&... args) {
				format = fmt::format(fmt::runtime(message), std::forward<decltype(args)>(args)...);
						});
			return format;
		}

		/**
		 * @brief Formats a string using a format string and arguments.
		 *
		 * This function wraps around the `fmt::format` function, providing a consistent
		 * interface for string formatting within the codebase. It takes a format string
		 * and a variable number of arguments to format the string accordingly.
		 *
		 * @param message The format string.
		 * @param args The arguments to be substituted into the format string.
		 * @return The formatted string.
		 *
		 * @note This function only works with string literals. For runtime formatting, use the `RuntimeFormat` function.
		 *
		 * @example Util::Format("Hello, {}", "world!");  // Returns "Hello, world!"
		 */
		template <typename... Args>
		std::string Format(fmt::format_string<Args...> fmt, Args&&... args)
		{

			auto str = fmt::format(fmt, std::forward<Args>(args)...);
			return str;
		}


	}

}