#include <NGIN/Logging/AsyncLogger.hpp>
#include <chrono>
#include <thread>
namespace NGIN::Logging
{



	AsyncLogger::AsyncLogger(std::chrono::milliseconds flushInterval)
		: flushInterval(flushInterval), exitFlag(false), workerThread([this]() { ProcessQueue(); })
	{}
	AsyncLogger::~AsyncLogger()
	{
		exitFlag.store(true);
		condVar.notify_one(); // Notify the logger thread to stop
		workerThread.join();
	}
	void AsyncLogger::LogInternal(Level level, const std::string& message)
	{
		{
			std::lock_guard<std::mutex> lock(bufferMutex);
			logBuffer.push_back({ level, message });
		}
		condVar.notify_one();
	}

	size_t AsyncLogger::GetBufferSize()
	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		return logBuffer.size();
	}

	void AsyncLogger::Flush()
	{


		for (const auto& sink : sinks)
		{
			for (const auto& entry : logBuffer)
			{
				sink->Log(entry);
			}
			sink->Flush();
		}
		logBuffer.clear();

	}

	void AsyncLogger::ProcessQueue()
	{
		while (!exitFlag.load())
		{
			std::unique_lock<std::mutex> lock(bufferMutex);
			if (condVar.wait_for(lock, flushInterval, [this] { return !logBuffer.empty() || exitFlag; }))
			{
				Flush();
			}
		}
		Flush();  // Final flush before exiting

	}
}
