#include <Core/Logging/AsyncLogger.hpp>

namespace NGIN
{
	AsyncLogger::AsyncLogger()
		: logQueue(), exitFlag(false), workerThread([this]() { ProcessQueue(); })
	{}
	AsyncLogger::~AsyncLogger()
	{
		exitFlag.store(true);
		condVar.notify_all();
		workerThread.join();
	}
	void AsyncLogger::LogInternal(LogLevel level, const std::string& message)
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		logQueue.push({ level, message });
		condVar.notify_one();
	}
	void AsyncLogger::ProcessQueue()
	{
		while (!exitFlag.load())
		{
			LogEntry logEntry;
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				condVar.wait(lock, [this] { return !logQueue.empty() || exitFlag.load(); });

				if (exitFlag.load() && logQueue.empty())
					return;
				while (!logQueue.empty())
				{
					logEntry = logQueue.front();
					logQueue.pop();

					for (auto& sink : sinks)
						sink->Log(logEntry.level, logEntry.message);

				}

				for (auto& sink : sinks)
					sink->Flush();

			}

		}
	}
}