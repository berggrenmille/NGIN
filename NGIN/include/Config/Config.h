#pragma once
#include <string>
#include <mutex>
#include <unordered_map>
#include <source_location>
#include <Core.h>
/**
 * @namespace NGIN::Config
 * @brief The namespace encapsulates the configuration management functionality.
 */
namespace NGIN
{
	namespace Config
	{

		/**
		 * @brief Loads the configuration from a JSON file
		 *
		 * Reads the JSON file into an internal unordered map for fast access. If the file does not exist,
		 * an empty map is created.
		 */
		NGIN_API void Load();

		/**
		 * @brief Saves the current configuration to a JSON file
		 *
		 * Writes the internal unordered map into a JSON file.
		 */
		NGIN_API void Save();

		/**
		 * @brief Fetches a configuration value
		 * @param key The key of the configuration entry
		 * @return The value of the configuration entry. Returns an empty string if the key is not found.
		 */
		NGIN_API std::string Get(const std::string& key, const std::source_location& source = std::source_location::current());

		/**
		 * @brief Sets a configuration value
		 * @param key The key of the configuration entry
		 * @param value The new value to be set for the key
		 */
		NGIN_API void Set(const std::string& key, const std::string& value);
	}
}
