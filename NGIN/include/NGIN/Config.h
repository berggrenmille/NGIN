#pragma once

#include <NGIN/Core.h>
#include <Detail/ConfigHelper.hpp>

#include <string>
#include <mutex>
#include <unordered_map>
#include <source_location>

namespace NGIN
{
	/**
	 * @namespace NGIN::Config
	 * @brief The namespace encapsulates the configuration management functionality.
	 */
	namespace Config
	{

		/**
		 * @brief Loads the configuration from a JSON file
		 *
		 * Reads the JSON file into an internal unordered map for fast access. If the file does not exist,
		 * an empty map is created.
		 */
		NGIN_API void Init();

		/**
		 * @brief Saves the current configuration to a JSON file
		 *
		 * Writes the internal unordered map into a JSON file.
		 */
		NGIN_API void Save();

		/**
		 * @brief Fetches a configuration value as a string
		 * @param key The key of the configuration entry
		 * @return The value of the configuration entry. Returns an empty string if the key is not found.
		 */
		NGIN_API std::string GetRawValue(const std::string& key, const std::source_location& source = std::source_location::current());

		/**
		 * @brief Sets a configuration value
		 * @param key The key of the configuration entry
		 * @param value The new value to be set for the key
		 */
		NGIN_API void Set(const std::string& key, const std::string& value);


		/**
		 * @brief Fetches a configuration value an converts it to the specified type
		 * @tparam T The type to convert the configuration value to
		 * @param key The key of the configuration entry
		 * @return The value of the configuration entry. Returns an empty string if the key is not found.
		 */
		template <typename T>
		T Get(const std::string& key, const std::source_location& source = std::source_location::current())
		{
			std::string value = GetRawValue(key, source);
			if (!value.empty())
				return Detail::FromString<T>(value, source);
			NGIN_ASSERT_SRC(false, source, "Config value not found for key: {}", key.c_str());
		}

	}
}
