#include <gtest/gtest.h>
#include <NGIN/Util/JSON.hpp>

using namespace NGIN::Util;

class JSONTest : public ::testing::Test
{
protected:
	// Some sample JSON strings for testing.
	const std::string validJSON = R"({ "name": "John", "age": 30, "city": "New York" })";
	const std::string nestedJSON = R"({ "person": { "name": "John", "age": 30 }, "city": "New York" })";
	const std::string invalidJSON = "{ 'name': 'John', 'age': 30, 'city': 'New York' }";
};

TEST_F(JSONTest, ParseValidJSON)
{
	JSON json;
	bool success = json.Parse(validJSON);
	EXPECT_TRUE(success);
}

TEST_F(JSONTest, ParseInvalidJSON)
{
	JSON json;
	bool success = json.Parse(invalidJSON);
	EXPECT_FALSE(success);
}

TEST_F(JSONTest, GetStringValue)
{
	JSON json(validJSON);
	std::string name;
	bool success = json.Get("name", name);
	EXPECT_TRUE(success);
	EXPECT_EQ(name, "John");
}

TEST_F(JSONTest, GetIntValue)
{
	JSON json(validJSON);
	int age;
	bool success = json.Get("age", age);
	EXPECT_TRUE(success);
	EXPECT_EQ(age, 30);
}

TEST_F(JSONTest, GetNestedObject)
{
	JSON json(nestedJSON);
	JSON person = json.GetObject("person");
	std::string name;
	bool success = person.Get("name", name);
	EXPECT_TRUE(success);
	EXPECT_EQ(name, "John");
}

TEST_F(JSONTest, SetStringValue)
{
	JSON json;
	json.Set("name", "John");
	std::string output = json.Dump();
	EXPECT_NE(output.find("John"), std::string::npos);
}

TEST_F(JSONTest, SetIntValue)
{
	JSON json;
	json.Set("age", 30);
	std::string output = json.Dump();
	EXPECT_NE(output.find("30"), std::string::npos);
}

TEST_F(JSONTest, SetNestedObject)
{
	JSON person;
	person.Set("name", "John");
	person.Set("age", 30);

	JSON json;
	json.SetObject("person", person);

	std::string output = json.Dump();
	EXPECT_NE(output.find("John"), std::string::npos);
	EXPECT_NE(output.find("30"), std::string::npos);
}

// Additional
