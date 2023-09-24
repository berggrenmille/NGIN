#include <NGIN/Util/JSON.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace NGIN::Util
{
	JSON::JSON() : data()
	{
		data.SetObject();
	}

	JSON::JSON(const String& jsonString)
	{
		Parse(jsonString);
	}

	// Copy constructor
	JSON::JSON(const JSON& other)
	{
		// You need to implement the actual copy logic here,
		// possibly utilizing rapidjson's functionalities for deep copying.
		this->data.CopyFrom(other.data, data.GetAllocator());
	}

	// Move constructor
	JSON::JSON(JSON&& other) noexcept
	{
		// Implement the move logic
		this->data = std::move(other.data);
	}

	// Copy assignment operator
	JSON& JSON::operator=(const JSON& other)
	{
		if (this != &other)
		{
			// Implement the actual copy logic here
			this->data.CopyFrom(other.data, data.GetAllocator());
		}
		return *this;
	}

	// Move assignment operator
	JSON& JSON::operator=(JSON&& other) noexcept
	{
		if (this != &other)
		{
			// Implement the move logic here
			this->data = std::move(other.data);
		}
		return *this;
	}

	Bool JSON::Parse(const String& jsonString)
	{
		data.Parse(jsonString.c_str());
		return !data.HasParseError();
	}

	Bool JSON::Contains(const String& key) const
	{
		return data.HasMember(key.c_str());
	}

	Bool JSON::IsEmpty() const
	{
		return data.ObjectEmpty();
	}

	JSON JSON::GetObject(const String& key) const
	{

		if (Contains(key) && data[key.c_str()].IsObject())
		{
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			data[key.c_str()].Accept(writer);
			return JSON(buffer.GetString());
		}

		return JSON();
	}

	void JSON::SetObject(const String& key, const JSON& value)
	{
		// Return if key is empty
		if (key.empty())
			return;

		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v;
		v.CopyFrom(value.data, data.GetAllocator());

		SetInternal(k, v);
	}

	String JSON::Dump() const
	{
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		data.Accept(writer);
		return String(buffer.GetString());
	}

	void JSON::SetInternal(rapidjson::Value& k, rapidjson::Value& v)
	{
		auto memberItr = data.FindMember(k);

		if (memberItr != data.MemberEnd())
			memberItr->value = v;
		else
			data.AddMember(k, v, data.GetAllocator());
	}
	template <>
	String JSON::Get<String>(const String& key) const
	{
		if (data.HasMember(key.c_str()) && data[key.c_str()].IsString())
			return data[key.c_str()].GetString();
		return "";
	}

	template <>
	Int32 JSON::Get<Int32>(const String& key) const
	{
		if (data.HasMember(key.c_str()) && data[key.c_str()].IsInt())
			return data[key.c_str()].GetInt();
		return 0;
	}

	template <>
	F32 JSON::Get<F32>(const String& key) const
	{
		if (data.HasMember(key.c_str()) && data[key.c_str()].IsFloat())
			return data[key.c_str()].GetFloat();
		return 0.0f;
	}

	template <>
	F64 JSON::Get<F64>(const String& key) const
	{
		if (data.HasMember(key.c_str()) && data[key.c_str()].IsDouble())
			return data[key.c_str()].GetDouble();
		return 0.0;
	}

	template <>
	UInt64 JSON::Get<UInt64>(const String& key) const
	{
		if (data.HasMember(key.c_str()) && data[key.c_str()].IsUint64())
			return data[key.c_str()].GetUint64();
		return 0;
	}

	template <>
	Bool JSON::Get<Bool>(const String& key) const
	{
		if (data.HasMember(key.c_str()) && data[key.c_str()].IsBool())
			return data[key.c_str()].GetBool();
		return 0;
	}

	template <>
	void JSON::Set<String>(const String& key, const String& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value.c_str(), data.GetAllocator());

		SetInternal(k, v);
	}

	template <>
	void JSON::Set<Int32>(const String& key, const Int32& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value);
		SetInternal(k, v);
	}

	template <>
	void JSON::Set<Int64>(const String& key, const Int64& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value);
		SetInternal(k, v);
	}

	template <>
	void JSON::Set<F32>(const String& key, const F32& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value);
		SetInternal(k, v);
	}

	template <>
	void JSON::Set<F64>(const String& key, const F64& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value);
		SetInternal(k, v);
	}

	template <>
	void JSON::Set<UInt64>(const String& key, const UInt64& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value);
		SetInternal(k, v);
	}

	template <>
	void JSON::Set<Bool>(const String& key, const Bool& value)
	{
		rapidjson::Value k(key.c_str(), data.GetAllocator());
		rapidjson::Value v(value);
		SetInternal(k, v);
	}
}
