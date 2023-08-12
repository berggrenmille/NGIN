#include <string>

namespace NGIN
{





	/**
	 * @brief Enum class representing the log severity/level.
	 */
	enum class LogLevel
	{
		Trace,    ///< Trace-level log, usually very detailed.
		Info,     ///< Informational messages, general knowledge about system operations.
		Debug,    ///< Used during debugging to log additional info.
		Warning,  ///< Represents warnings that aren't critical but might indicate potential issues.
		Error,    ///< Represents errors that aren't fatal but might impact normal operation.
		Critical  ///< Critical errors that might cause the system to terminate.
	};

	/**
	*  @brief Representation for all data a sink needs to print
	*/
	struct LogEntry
	{
		LogLevel level;
		std::string message;
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


		/**
		 * @brief Flushes the sink and ensures all pending messages are outputted.
		 */
		virtual void Flush() {};



	};

}  // namespace NGIN
