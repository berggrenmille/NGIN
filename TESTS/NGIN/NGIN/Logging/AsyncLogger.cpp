#include <gtest/gtest.h>
#include <NGIN/Logging/AsyncLogger.hpp>
#include <array>
#include <thread>

using namespace NGIN::Logging;

class AsyncLoggerTest : public ::testing::Test
{
protected:
	std::unique_ptr<AsyncLogger> logger;

	void SetUp() override
	{
		logger = std::make_unique<AsyncLogger>(std::chrono::milliseconds(10));
	}
};

TEST_F(AsyncLoggerTest, BufferEmptyAfterInterval)
{
	logger->Log(Level::Info, "Test log.");
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	// Assuming you can get the current buffer content or size
	EXPECT_EQ(logger->GetBufferSize(), 0);
}

TEST_F(AsyncLoggerTest, MultiThreadedBufferEmptyAfterInterval)
{
	constexpr int THREAD_COUNT = 5;
	constexpr int LOGS_PER_THREAD = 100;

	auto logTask = [&]() {
		for (int i = 0; i < LOGS_PER_THREAD; ++i)
		{
			logger->Log(Level::Info, "Test log from thread.");
		}
	};

	std::array<std::thread, THREAD_COUNT> threads;
	for (int i = 0; i < THREAD_COUNT; ++i)
		threads[i] = std::thread(logTask);

	for (auto& t : threads)
	{
		t.join();
	}

	// Allow logger to process
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	EXPECT_EQ(logger->GetBufferSize(), 0);  // All logs should be processed
}
