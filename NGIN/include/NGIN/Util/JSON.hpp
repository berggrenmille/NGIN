#pragma once
#include <NGIN/Defines.hpp>
#include <HideWarnings/Json.hpp>
#include <string>
#include <type_traits>

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
		NGIN_API JSON(const String &jsonString);

		/**
		 * @brief Copy constructor for JSON.
		 * @param other The JSON object to copy from.
		 */
		NGIN_API JSON(const JSON &other);

		/**
		 * @brief Move constructor for JSON.
		 * @param other The JSON object to move from.
		 */
		NGIN_API JSON(JSON &&other) noexcept;

		/**
		 * @brief Copy assignment operator for JSON.
		 * @param other The JSON object to copy from.
		 * @return Returns *this.
		 */
		NGIN_API JSON &operator=(const JSON &other);

		/**
		 * @brief Move assignment operator for JSON.
		 * @param other The JSON object to move from.
		 * @return Returns *this.
		 */
		NGIN_API JSON &operator=(JSON &&other) noexcept;

		/**
		 * @brief Parses a string representation into a JSON object.
		 * @param jsonString The string representation of the JSON.
		 * @return Returns true if the parse was successful, otherwise false.
		 */
		NGIN_API Bool Parse(const String &jsonString);

		/**
		 * @brief Checks if the JSON object contains a given key.
		 * @param key The key to check for.
		 * @return Returns true if the key is found, otherwise false.
		 */
		NGIN_API Bool Contains(const String &key) const;

		/**
		 * @brief Checks if the JSON object is empty.
		 * @return Returns true if the JSON object is empty, otherwise false.
		 */
		NGIN_API Bool IsEmpty() const;

		/**
		 * @brief Retrieves a nested JSON object associated with a given key.
		 * @param key The key associated with the desired JSON object.
		 * @return Returns the JSON object if the key is found and it corresponds to an object, otherwise returns an empty JSON object.
		 */
		NGIN_API JSON GetObject(const String &key) const;

		/**
		 * @brief Retrieves a value of a specific type T associated with a given key.
		 *
		 * @tparam T The type of value to retrieve.
		 * @param key The key associated with the value.
		 * @return The value of the specified type T if the key exists and the value can be converted to T.
		 *         Otherwise returns a default-constructed value of T.
		 */
		template <typename T>
		T Get(const String &key) const;

		// Explicit template specializations
		template <>
		NGIN_API String Get<String>(const String &key) const;

		template <>
		NGIN_API Int32 Get<Int32>(const String &key) const;

		template <>
		NGIN_API Int64 Get<Int64>(const String &key) const;

		template <>
		NGIN_API F32 Get<F32>(const String &key) const;

		template <>
		NGIN_API F64 Get<F64>(const String &key) const;

		template <>
		NGIN_API UInt64 Get<UInt64>(const String &key) const;

		template <>
		NGIN_API Bool Get<Bool>(const String &key) const;

		/**
		 * @brief Sets a value of a specific type T for a given key.
		 *
		 * @tparam T The type of value to set.
		 * @param key The key to associate with the value.
		 * @param value The value to set.
		 */
		template <typename T>
		void Set(const String &key, const T &value);
		// Explicit template specializations
		template <>
		NGIN_API void Set<String>(const String &key, const String &value);

		template <>
		NGIN_API void Set<Int32>(const String &key, const Int32 &value);

		template <>
		NGIN_API void Set<Int64>(const String &key, const Int64 &value);

		template <>
		NGIN_API void Set<F32>(const String &key, const F32 &value);

		template <>
		NGIN_API void Set<F64>(const String &key, const F64 &value);

		template <>
		NGIN_API void Set<UInt64>(const String &key, const UInt64 &value);

		template <>
		NGIN_API void Set<Bool>(const String &key, const Bool &value);

		/**
		 * @brief Sets a nested JSON object in the current JSON object associated with a given key.
		 * @param key The key to be associated with the given JSON object.
		 * @param value The JSON object to be set.
		 */
		NGIN_API void SetObject(const String &key, const JSON &value);

		/**
		 * @brief Converts the JSON object into its string representation.
		 * @return Returns the string representation of the JSON object.
		 */
		NGIN_API String Dump() const;

	private:
		NGIN_API void SetInternal(rapidjson::Value &k, rapidjson::Value &v);
		/// The internal representation of the JSON data.
		rapidjson::Document data;
	};

}
