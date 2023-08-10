#include <gtest/gtest.h>
#include <Core/Config.h>

using namespace NGIN;

class ConfigTest : public ::testing::Test
{
protected:

	// This function runs before every test case.
	virtual void SetUp() override
	{
		// Initialize the configuration before each test
		Config::Init();
	}

	// This function runs after every test case.
	virtual void TearDown() override
	{
		// Cleanup or any other required functionality
	}
};

TEST_F(ConfigTest, Initialization)
{
	std::string value = Config::GetRawValue("TEST");
	EXPECT_EQ(value, "TEST");
}

TEST_F(ConfigTest, NonExistentKey)
{
	std::string value = Config::GetRawValue("NON_EXISTENT_KEY");
	EXPECT_EQ(value, "");
}

TEST_F(ConfigTest, SetAndGet)
{
	Config::Set("NEW_KEY", "NEW_VALUE");
	std::string value = Config::GetRawValue("NEW_KEY");
	EXPECT_EQ(value, "NEW_VALUE");
}

TEST_F(ConfigTest, OverwriteValue)
{
	Config::Set("TEST", "OVERWRITTEN");
	std::string value = Config::GetRawValue("TEST");
	EXPECT_EQ(value, "OVERWRITTEN");
}

TEST_F(ConfigTest, GetWithTemplate)
{
	// Assuming your Detail::FromString function has specialization for int, for the purpose of this test.
	Config::Set("INT_KEY", "42");
	int value = Config::Get<int>("INT_KEY");
	EXPECT_EQ(value, 42);
}

// Further tests can be added as needed.

