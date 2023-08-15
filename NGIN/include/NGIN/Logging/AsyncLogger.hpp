#pragma once
#include "BaseLogger.hpp"
#include "Defines.hpp"

#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace NGIN
{
	namespace Logging
	{


		/**
		 * @class AsyncLogger
		 * @brief Asynchronous logger implementation.
		 *
		 * This class provides an asynchronous logging mechanism. Log messages are buffered
		 * and flushed to the sinks either when the buffer reaches a certain size or at regular
		 * intervals defined by `flushInterval`.
		 *
		 * The asynchronous nature ensures that the calling thread does not block due to logging
		 * operations, making it suitable for performance-critical applications.
		 *
		 * @note This class is derived from the `BaseLogger` and overrides its `LogInternal` method.
		 *
		 * @see BaseLogger
		 */
		class AsyncLogger : public BaseLogger
		{
		public:
			/**
			 * @brief Construct an asynchronous logger.
			 *
			 * Initializes the asynchronous logger with the provided flush interval or
			 * defaults to 5 milliseconds if none is provided.
			 *
			 * @param flushInterval The time interval at which the buffer should be flushed.
			 *                      Default is 5 milliseconds.
			 */
			NGIN_API AsyncLogger(std::chrono::milliseconds flushInterval = std::chrono::milliseconds(5));

			/**
			 * @brief Destructor.
			 *
			 * Ensures that all buffered log messages are flushed and cleans up the resources.
			 */
			NGIN_API ~AsyncLogger();

			NGIN_API size_t GetBufferSize();

		protected:
			/**
			 * @brief Implements the actual logging logic.
			 *
			 * This function is invoked when a log message needs to be processed. Instead of
			 * writing the message directly to the sinks, it buffers the message and writes
			 * asynchronously based on the flush interval.
			 *
			 * @param level The log level of the message.
			 * @param message The log message to be processed.
			 */
			NGIN_API void LogInternal(Level level, const std::string& message) override;

		private:
			/**
			 * @brief Process log messages from the buffer.
			 *
			 * Periodically invoked to process log messages buffered for asynchronous writing.
			 */
			NGIN_API void ProcessQueue();

			/**
			 * @brief Flush the log buffer to the sinks.
			 *
			 * Sends all the buffered log messages to the attached sinks.
			 */
			NGIN_API void Flush();


			std::vector<Entry> logBuffer;           ///< Buffer for storing log messages.
			std::condition_variable condVar;        ///< Condition variable for signaling the worker thread.
			std::chrono::milliseconds flushInterval;///< Interval at which the log buffer should be flushed.
			std::mutex bufferMutex;                 ///< Mutex for protecting concurrent access to `logBuffer`.
			std::atomic<bool> exitFlag;             ///< Flag indicating when the logger should stop processing.
			std::thread workerThread;               ///< Thread responsible for asynchronous logging operations.
		};

	}
}