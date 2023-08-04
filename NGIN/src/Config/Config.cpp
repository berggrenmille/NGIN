#include <Config/Config.h>
#include <HideWarnings/Json.hpp>
#include <fstream>
#include <Logger.h>

namespace
{
	/// Mutex to guard against concurrent access to the configMap
	std::mutex configMutex;
	/// An unordered map to hold the configuration data
	std::unordered_map<std::string, std::string> configMap;
	/// The name of the configuration file
	const std::string configFile = "config.json";
}

namespace NGIN
{

	NGIN_API void Config::Load()
	{
		std::lock_guard<std::mutex> lock(configMutex);
		std::ifstream file(configFile);
		if (file)
		{
			nlohmann::json j;
			file >> j;
			for (auto it = j.begin(); it != j.end(); ++it)
			{
				configMap[it.key()] = it.value().get<std::string>();
			}
		}
	}

	NGIN_API void Config::Save()
	{
		std::lock_guard<std::mutex> lock(configMutex);
		nlohmann::json j = nlohmann::json(configMap);
		std::ofstream file(configFile);
		if (file)
		{
			file << j.dump(4);
		}
	}

	NGIN_API std::string Config::Get(const std::string& key, const std::source_location& source)
	{
		std::lock_guard<std::mutex> lock(configMutex);
		if (configMap.find(key) != configMap.end())
		{
			return configMap[key];
		}
		Logger::Log(source, Logger::ERROR, "Tried to get unknown config key: %s", key.c_str());
		return "";
	}

	NGIN_API void Config::Set(const std::string& key, const std::string& value)
	{
		std::lock_guard<std::mutex> lock(configMutex);
		configMap[key] = value;
	}

}
