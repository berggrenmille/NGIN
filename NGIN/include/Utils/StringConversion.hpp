#pragma once
#include "Core/Logger.h"

#include <iostream>
#include <sstream>
#include <string>

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
			NGIN_ASSERT_SRC(!ss.fail(), source, "Failed to convert \"{}\" to type: {}", s.c_str(), typeid(T).name());
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
		std::string FromString<std::string>(const std::string& s, const std::source_location& source)
		{
			return s;
		}



	}

}