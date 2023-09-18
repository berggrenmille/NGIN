#include <gtest/gtest.h>
#include <NGIN/Logging/FileSink.hpp>
#include <fstream>
#include <string>

using namespace NGIN::Logging;

class FileSinkTest : public ::testing::Test
{
protected:
	std::string testFilename = "test_log.txt";

	virtual void SetUp() override
	{
		// Delete test file at start of each test, to ensure a clean slate
		std::remove(testFilename.c_str());
	}

	virtual void TearDown() override
	{
		// Delete test file after each test, for cleanliness
		std::remove(testFilename.c_str());
	}

	bool FileContains(const std::string& content)
	{
		std::ifstream file(testFilename);
		std::string line;
		while (std::getline(file, line))
		{
			if (line == content)
				return true;
		}
		return false;
	}
};

TEST_F(FileSinkTest, BasicFileCreation)
{
	FileSink sink(testFilename);
	sink.Init();
	EXPECT_TRUE(std::ifstream(testFilename).is_open());
	sink.Cleanup();
}

TEST_F(FileSinkTest, BasicLogging)
{
	FileSink sink(testFilename);
	sink.Init();
	sink.Log({ Level::Info, "Test Log Message" });
	sink.Flush();
	EXPECT_TRUE(FileContains("Test Log Message"));
	sink.Cleanup();
}

TEST_F(FileSinkTest, MultipleLogging)
{
	FileSink sink(testFilename);
	sink.Init();
	sink.Log({ Level::Info, "Message 1" });
	sink.Log({ Level::Error, "Message 2" });
	sink.Flush();
	EXPECT_TRUE(FileContains("Message 1"));
	EXPECT_TRUE(FileContains("Message 2"));
	sink.Cleanup();
}

TEST_F(FileSinkTest, AutoFlushOnCleanup)
{
	FileSink sink(testFilename);
	sink.Init();
	sink.Log({ Level::Info, "AutoFlush Test" });
	sink.Cleanup();
	EXPECT_TRUE(FileContains("AutoFlush Test"));
}

// Add more tests as required...
