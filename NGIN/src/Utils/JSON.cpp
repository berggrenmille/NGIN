#include <Utils/JSON.hpp>
namespace NGIN::Util
{


	JSON::JSON() {}

	JSON::JSON(const std::string& jsonString)
	{
		Parse(jsonString);
	}

	bool JSON::Parse(const std::string& jsonString)
	{
		data = nlohmann::json::parse(jsonString, nullptr, false);
		return !data.is_discarded();
	}



	JSON JSON::GetObject(const std::string& key) const
	{
		if (data.contains(key) && data[key].is_object())
		{
			return JSON(data[key].dump());
		}
		return JSON();
	}

	void JSON::Set(const std::string& key, const std::string& value)
	{
		data[key] = value;
	}

	void JSON::SetObject(const std::string& key, const JSON& value)
	{
		data[key] = nlohmann::json::parse(value.Dump());
	}

	std::string JSON::Dump() const
	{
		return data.dump();
	}
}