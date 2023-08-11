#include <string>
#include "LogDefinitions.hpp"
namespace NGIN
{

	/**
	 * @brief Interface for all log sinks within the NGIN framework.
	 */
	class ILogSink
	{
	public:
		/**
		 * @brief Virtual destructor for interface cleanup.
		 */
		virtual ~ILogSink() = default;

		/**
		 * @brief Processes and logs a message to the specified sink.
		 *
		 * @param level The severity/level of the log message.
		 * @param message The log message to be processed.
		 */
		virtual void logMessage(LogLevel level, const std::string& message) = 0;
	};

}  // namespace NGIN
