#include <gtest/gtest.h>
#include <Core/Logger.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace NGIN;

namespace
{
	const std::string TEST_LOG_FILE = "TestNGIN.log";
}

class LoggerTest : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
		// Redirect loguru output to a test file before each test
		loguru::add_file(TEST_LOG_FILE.c_str(), loguru::Truncate, loguru::Verbosity_MAX);
	}

	virtual void TearDown() override
	{
		// Delete the test log file after each test
		std::remove(TEST_LOG_FILE.c_str());

	}

	// Helper function to read the log file
	std::string readLogFile()
	{
		std::ifstream file(TEST_LOG_FILE);
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
