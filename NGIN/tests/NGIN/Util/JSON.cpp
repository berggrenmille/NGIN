#include <gtest/gtest.h>
#include <NGIN/Util/JSON.hpp>

using namespace NGIN::Util;

class JSONTest : public ::testing::Test
{
protected:
	// Sample JSON string literals for testing.
	static constexpr const char *validJSONString = "{\"name\": \"Alice\", \"age\": 30}";
	static constexpr const char *invalidJSONString = "{\"name\": \"Alice, \"age\": 30}";
};

TEST_F(JSONTest, ParseValidJSON)
{
	JSON json;
	EXPECT_TRUE(json.Parse(validJSONString));
}

TEST_F(JSONTest, ParseInvalidJSON)
{
	JSON json;
	EXPECT_FALSE(json.Parse(invalidJSONString));
}

TEST_F(JSONTest, ContainsKey)
{
	JSON json(validJSONString);
	EXPECT_TRUE(json.Contains("name"));
	EXPECT_FALSE(json.Contains("gender"));
}

TEST_F(JSONTest, IsEmpty)
{
	JSON json;
	EXPECT_TRUE(json.IsEmpty());
	json.Parse(validJSONString);
	EXPECT_FALSE(json.IsEmpty());
}

TEST_F(JSONTest, GetObject)
{
	JSON json("{\"person\": {\"name\": \"Alice\", \"age\": 30}}");
	JSON person = json.GetObject("person");
	EXPECT_FALSE(person.IsEmpty());
}

TEST_F(JSONTest, GetInvalidObject)
{
	JSON json(validJSONString);
	JSON invalidObject = json.GetObject("name");
	EXPECT_TRUE(invalidObject.IsEmpty());
}

TEST_F(JSONTest, GetNonExistingObject)
{
	JSON json(validJSONString);
	JSON nonExisting = json.GetObject("gender");
	EXPECT_TRUE(nonExisting.IsEmpty());
}

TEST_F(JSONTest, GetStringValue)
{
	JSON json(validJSONString);
	EXPECT_EQ(json.Get<std::string>("name"), "Alice");
}

TEST_F(JSONTest, GetIntValue)
{
	JSON json(validJSONString);
	EXPECT_EQ(json.Get<int>("age"), 30);
}

TEST_F(JSONTest, GetMissingKey)
{
	JSON json(validJSONString);
	EXPECT_EQ(json.Get<std::string>("gender"), "");
}

TEST_F(JSONTest, SetStringValue)
{
	JSON json;
	json.Set<std::string>("name", "Bob");
	EXPECT_TRUE(json.Contains("name"));
}

TEST_F(JSONTest, SetIntValue)
{
	JSON json;
	json.Set<int>("age", 25);
	EXPECT_TRUE(json.Contains("age"));
}

TEST_F(JSONTest, SetObject)
{
	JSON person(validJSONString);
	JSON json;
	json.SetObject("person", person);
	EXPECT_TRUE(json.Contains("person"));
}

TEST_F(JSONTest, Dump)
{
	JSON json(validJSONString);
	std::string dumped = json.Dump();
	EXPECT_FALSE(dumped.empty());
}

TEST_F(JSONTest, InitializationFromAnotherJSON)
{
	JSON original(validJSONString);
	JSON copy(original);

	EXPECT_EQ(original.Get<std::string>("name"), copy.Get<std::string>("name"));
	EXPECT_EQ(original.Get<int>("age"), copy.Get<int>("age"));
}

TEST_F(JSONTest, SetAndGetNestedObject)
{
	JSON person(validJSONString);
	JSON root;
	root.SetObject("person", person);

	JSON retrievedPerson = root.GetObject("person");
	EXPECT_EQ(retrievedPerson.Get<std::string>("name"), "Alice");
}

TEST_F(JSONTest, ParseEmptyString)
{
	JSON json;
	EXPECT_FALSE(json.Parse(""));
}

TEST_F(JSONTest, ParseOnlyWhitespaces)
{
	JSON json;
	EXPECT_FALSE(json.Parse("  "));
}

TEST_F(JSONTest, OverwriteExistingKey)
{
	JSON json(validJSONString);
	json.Set<std::string>("name", "Bob");
	EXPECT_EQ(json.Get<std::string>("name"), "Bob");
}

TEST_F(JSONTest, GetWithSpecialCharacters)
{
	JSON json("{\"special_key!@#\": 42}");
	EXPECT_EQ(json.Get<int>("special_key!@#"), 42);
}

TEST_F(JSONTest, InvalidTypeAccess)
{
	JSON json(validJSONString);
	EXPECT_EQ(json.Get<int>("name"), 0); // Assuming it returns default-constructed value
}

TEST_F(JSONTest, CaseSensitivity)
{
	JSON json(validJSONString);
	EXPECT_FALSE(json.Contains("Name"));
}

TEST_F(JSONTest, UnicodeCharacters)
{
	JSON json("{\"unicode\": \"\\u0041lice\"}");
	EXPECT_EQ(json.Get<std::string>("unicode"), "Alice");
}

TEST_F(JSONTest, SettingNullValue)
{
	JSON json;
	json.SetObject("nullKey", JSON()); // Assuming setting empty JSON sets it to null
	EXPECT_TRUE(json.Contains("nullKey"));
}

TEST_F(JSONTest, DumpEmptyObject)
{
	JSON json;
	std::string dumped = json.Dump();
	EXPECT_EQ(dumped, "{}");
}
