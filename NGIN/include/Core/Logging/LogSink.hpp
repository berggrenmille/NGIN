#include <string>
#include "LogDefinitions.hpp"
namespace NGIN
{

	/**
	*  @brief Representation for message data
	*/
	struct LogEntry
	{
		LogLevel level;
		std::string message;
	};

	/**
	 * @brief Enum class representing the log severity/level.
	 */
	enum class LogLevel
	{
		TRACE,    ///< Trace-level log, usually very detailed.
		INFO,     ///< Informational messages, general knowledge about system operations.
		DEBUG,    ///< Used during debugging to log additional info.
		WARNING,  ///< Represents warnings that aren't critical but might indicate potential issues.
		ERROR,    ///< Represents errors that aren't fatal but might impact normal operation.
		CRITICAL  ///< Critical errors that might cause the system to terminate.
	};


	/**
	 * @brief Interface for all log sinks within the NGIN framework.
	 *
	 * Sinks are responsible for handling log messages and outputting them to the appropriate
	 *
	 */
	class LogSink
	{
	public:
		/**
		 * @brief Virtual destructor for interface cleanup.
		 */
		virtual ~LogSink() = default;

		/**
		 * @brief Initializes the log sink.
		 *
		 * @return true if initialization was successful, false otherwise.
		 */
		virtual bool Init() { return true; }

		/**
		 * @brief Cleans up resources associated with the log sink.
		 */
		virtual void Cleanup() {}

		/**
		 * @brief Processes and logs a message to the specified sink.
		 *
		 * @param level The severity/level of the log message.
		 * @param message The log message to be processed.
		 */
		virtual void Log(LogLevel level, const std::string& message) = 0;

		virtual void LogBatch(const std::vector<LogEntry>& entries)
		{
			for (const auto& entry : entries)
				Log(level, entry);
		};


	};

}  // namespace NGIN
