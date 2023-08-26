#pragma once
#include <NGIN/Core.h>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace NGIN::Util
{
	/**
	 * @brief A utility class for handling JSON data.
	 */
	class JSON
	{
	public:
		/**
		 * @brief Default constructor for JSON.
		 */
		NGIN_API JSON();

		/**
		 * @brief Constructs the JSON object from a string representation.
		 * @param jsonString A string representation of a JSON object.
		 */
		NGIN_API JSON(const std::string& jsonString);

		/**
		 * @brief Parses a string representation into a JSON object.
		 * @param jsonString The string representation of the JSON.
		 * @return Returns true if the parse was successful, otherwise false.
		 */
		NGIN_API bool Parse(const std::string& jsonString);

		/**
		 * @brief Retrieves a value of type T associated with a given key.
		 * @param key The key associated with the desired value.
		 * @param value A reference to a variable to store the retrieved value.
		 * @return Returns true if the key is found and the value is successfully retrieved, otherwise false.
		 */
		 // For arithmetic types:
		template<typename T,
			typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
		bool Get(const std::string& key, T& value) const;

		// For string types
		template<typename T,
			typename std::enable_if<std::is_same<T, std::string>::value, int>::type = 0>
		bool Get(const std::string& key, T& value) const;

		// For unknown types:
		template<typename T,
			typename std::enable_if<!std::is_arithmetic<T>::value &&
			!std::is_same<T, std::string>::value, int>::type = 0>
		bool Get(const std::string& key, T& value) const;




		/**
		 * @brief Retrieves a nested JSON object associated with a given key.
		 * @param key The key associated with the desired JSON object.
		 * @return Returns the JSON object if the key is found and it corresponds to an object, otherwise returns an empty JSON object.
		 */
		NGIN_API JSON GetObject(const std::string& key) const;

		/**
		 * @brief Sets a key-value pair in the JSON object of a generic type T.
		 * @param key The key to be associated with the given value.
		 * @param value The value to be set.
		 */
		template<typename T>
		void Set(const std::string& key, const T& value);

		/**
		 * @brief Sets a key-value pair in the JSON object with a string value.
		 * @param key The key to be associated with the given value.
		 * @param value The string value to be set.
		 */
		NGIN_API void Set(const std::string& key, const std::string& value);

		/**
		 * @brief Sets a nested JSON object in the current JSON object associated with a given key.
		 * @param key The key to be associated with the given JSON object.
		 * @param value The JSON object to be set.
		 */
		NGIN_API void SetObject(const std::string& key, const JSON& value);

		/**
		 * @brief Converts the JSON object into its string representation.
		 * @return Returns the string representation of the JSON object.
		 */
		NGIN_API std::string Dump() const;

	private:
		/// The internal representation of the JSON data.
		nlohmann::json data;
	};

	template<typename T,
		typename std::enable_if<std::is_arithmetic<T>::value, int>::type>
	bool JSON::Get(const std::string& key, T& value) const
	{
		if (data.contains(key) && data[key].is_number())
		{
			value = data[key].get<T>();
			return true;
		}
		return false;
	}

	template<typename T,
		typename std::enable_if<std::is_same<T, std::string>::value, int>::type>
	bool JSON::Get(const std::string& key, T& value) const
	{
		if (data.contains(key) && data[key].is_string())
		{
			value = data[key].get<std::string>();
			return true;
		}
		return false;
	}


	template<typename T,
		typename std::enable_if<!std::is_arithmetic<T>::value &&
		!std::is_same<T, std::string>::value, int>::type>
	bool JSON::Get(const std::string& key, T& value) const
	{
		return false;
	}

	template<typename T>
	void JSON::Set(const std::string& key, const T& value)
	{
		data[key] = value;
	}
}