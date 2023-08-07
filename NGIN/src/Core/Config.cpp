#include <PCH.h>
#include <Core/Config.h>
#include <HideWarnings/Json.hpp>
#include <fstream>

namespace
{
	/// Mutex to guard against concurrent access to the configMap
	std::mutex configMutex;
	/// An unordered map to hold the configuration data
	std::unordered_map<std::string, std::string> configMap;
	/// The name of the configuration file
	const std::string ConfigPath = "NGIN_CONFIG.json";

	nlohmann::json defaultConfigJSON = {
		{"TEST", "TEST"},
		{"TEST2", "TEST"}
	};

	void CreateConfigFile()
	{
		NGIN_WARNING("Creating config file: {}", ConfigPath.c_str());
		std::ofstream file(ConfigPath, std::ios::out | std::ios::app);
		NGIN_ASSERT(file.good(), "Failed to create config file: {}, {}", ConfigPath.c_str(), strerror(errno));
		file << defaultConfigJSON.dump(4);
		file.close();
	}
}

namespace NGIN
{

	NGIN_API void Config::Init()
	{
		std::lock_guard<std::mutex> lock(configMutex);

		std::ifstream file(ConfigPath);
		if (!file.is_open())
		{
			NGIN_WARNING("Error loading Config file at path: {} : {}", ConfigPath, strerror(errno));
			CreateConfigFile();
			file.open(ConfigPath);
			NGIN_ASSERT(file.is_open(), "Failed to read config file: {}", ConfigPath);
		}

		//Create JSON object and stream the config file into it
		nlohmann::json j;
		file >> j;
		file.close();

		//Load JSON data into local map
		for (const auto& [key, value] : j.items())
		{
			configMap[key] = value.get<std::string>();
		}
	}

	NGIN_API void Config::Save()
	{
		std::lock_guard<std::mutex> lock(configMutex);

		nlohmann::json j = nlohmann::json(configMap);

		std::ofstream file(ConfigPath);
		NGIN_ASSERT(file.is_open(), "Failed to write to config file: {}, ", ConfigPath, strerror(errno));
		file << j.dump(4);
	}

	NGIN_API std::string Config::GetRawValue(const std::string& key, const std::source_location& source)
	{
		std::lock_guard<std::mutex> lock(configMutex);
		if (configMap.find(key) != configMap.end())
		{
			return configMap[key];
		}
		return "";
	}



	NGIN_API void Config::Set(const std::string& key, const std::string& value)
	{
		std::lock_guard<std::mutex> lock(configMutex);
		configMap[key] = value;
	}

}
