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
		T FromString(const std::string &s, const std::source_location &source = std::source_location::current())
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
		inline std::string FromString<std::string>(const std::string &s, const std::source_location &source)
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
		inline const char *ExtractFileName(const char *fullPath)
		{
			// Find the last backslash, diffrent OS can use diffrent slashes
			const char *fromSlash = strrchr(fullPath, '/');
			const char *fromBackslash = strrchr(fullPath, '\\');

			// Determine which one is the last separator in the path.
			const char *lastSeparator = (fromSlash > fromBackslash) ? fromSlash : fromBackslash;

			return (lastSeparator) ? lastSeparator + 1 : fullPath;
		}

	}

}