#include <gtest/gtest.h>
#include <NGIN/Logger.h>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace NGIN;

class LoggerTest : public ::testing::Test
{
protected:
	std::string currentLogFile;  // Store the current test's log file name

	virtual void SetUp() override
	{
		// Generate a unique log file name based on the current test name
		currentLogFile = std::string(::testing::UnitTest::GetInstance()->current_test_info()->test_suite_name())
			+ "_"
			+ ::testing::UnitTest::GetInstance()->current_test_info()->name()
			+ ".log";

		// Replace any non-alphanumeric characters (like '/') in the test name with underscores
		std::replace_if(currentLogFile.begin(), currentLogFile.end(),
						[](char c) { return !std::isalnum(c) && c != '.'; },
						'_');

		// Remove all previous logging destinations
		loguru::remove_all_callbacks();

		// Redirect loguru output to the unique test file
		loguru::add_file(currentLogFile.c_str(), loguru::Truncate, loguru::Verbosity_MAX);
	}

	virtual void TearDown() override
	{
		loguru::remove_all_callbacks();
		if (std::remove(currentLogFile.c_str()) != 0)
		{
			std::perror("Error deleting file"); // This will print the error message related to file removal
		}
	}

	// Helper function to read the log file
	std::string readLogFile()
	{
		std::ifstream file(currentLogFile);
		std::stringstream logContent;
		logContent << file.rdbuf();
		return logContent.str();
	}
};

TEST_F(LoggerTest, BasicLogging)
{
	NGIN_INFO("Test message");
	std::string logContent = readLogFile();
	EXPECT_NE(logContent.find("Test message"), std::string::npos);
}

TEST_F(LoggerTest, LogWarning)
{
	NGIN_WARNING("Warning message");
	std::string logContent = readLogFile();
	EXPECT_NE(logContent.find("Warning message"), std::string::npos);
}

TEST_F(LoggerTest, LogError)
{
	NGIN_ERROR("Error message");
	std::string logContent = readLogFile();
	EXPECT_NE(logContent.find("Error message"), std::string::npos);
}

TEST_F(LoggerTest, LogWithArguments)
{
	NGIN_INFO("Test {} and {}", 123, "args");
	std::string logContent = readLogFile();
	EXPECT_NE(logContent.find("Test 123 and args"), std::string::npos);
}

TEST_F(LoggerTest, LogLevelVerbosity)
{
	NGIN_LOG(Logger::Verbosity::Verbosity_2, "Verbosity 2 message");
	std::string logContent = readLogFile();
	EXPECT_NE(logContent.find("Verbosity 2 message"), std::string::npos);

	NGIN_LOG(Logger::Verbosity::Verbosity_9, "Verbosity 9 message");
	logContent = readLogFile();
	EXPECT_NE(logContent.find("Verbosity 9 message"), std::string::npos);
}

TEST_F(LoggerTest, LogLocation)
{
	NGIN_INFO("Check location");
	std::string logContent = readLogFile();
	EXPECT_NE(logContent.find("Check location"), std::string::npos);
#ifdef _WIN32
	EXPECT_NE(logContent.find(strrchr(__FILE__, '\\') + 1), std::string::npos);
#else
	EXPECT_NE(logContent.find(strrchr(__FILE__, '/') + 1), std::string::npos);
#endif
}

// ... You can add more tests as required
