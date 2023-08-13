#pragma once
#include "BaseLogger.hpp"

#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace NGIN
{
	class NGIN_API AsyncLogger : public BaseLogger
	{
	public:
		AsyncLogger();
		~AsyncLogger();
	protected:
		void LogInternal(LogLevel level, const std::string& message) override;

	private:
		void ProcessQueue();

		std::queue<LogEntry, std::deque<LogEntry>> logQueue;
		std::condition_variable condVar;
		std::mutex queueMutex;
		std::atomic<bool> exitFlag;
		std::thread workerThread;
	};
}