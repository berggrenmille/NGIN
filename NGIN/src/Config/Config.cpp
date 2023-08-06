#include <Config/Config.h>
#include <HideWarnings/Json.hpp>
#include <fstream>

namespace
{
	/// Mutex to guard against concurrent access to the configMap
	std::mutex configMutex;
	/// An unordered map to hold the configuration data
	std::unordered_map<std::string, std::string> configMap;
	/// The name of the configuration file
	const std::string ConfigPath = "config.json";

	void CreateConfigFile()
	{
		NGIN_WARNING("Creating config file: {}", ConfigPath.c_str());
		std::ofstream file(ConfigPath, std::ios::out | std::ios::app);
		NGIN_ASSERT(file.good(), "Failed to create config file: {}, {}", ConfigPath.c_str(), strerror(errno));
		file << "{}";
		file.close();
	}
}

namespace NGIN
{

	NGIN_API void Config::Init()
	{
		std::lock_guard<std::mutex> lock(configMutex);
		std::ifstream file;
		file.open(ConfigPath);
		if (file.fail())
		{
			NGIN_WARNING("Error loading Config file at path: {} : {}", ConfigPath, strerror(errno));
			file.close();
			CreateConfigFile();
			file.open(ConfigPath);
			NGIN_ASSERT(file.good(), "Failed to read config file: {} : {}", ConfigPath.c_str(), strerror(errno));
		}


		nlohmann::json j;
		file >> j;
		for (auto it = j.begin(); it != j.end(); ++it)
		{
			configMap[it.key()] = it.value().get<std::string>();
		}

	}

	NGIN_API void Config::Save()
	{
		std::lock_guard<std::mutex> lock(configMutex);
		nlohmann::json j = nlohmann::json(configMap);
		std::ofstream file(ConfigPath);
		if (file)
		{
			file << j.dump(4);
		}
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
