#pragma once
#include <NGIN/Defines.hpp>
#include <string>
#include "Defines.hpp"
namespace NGIN::Logging
{
	/**
	 * @brief Interface for all log sinks within the NGIN framework.
	 *
	 * Sinks are responsible for handling log messages and outputting them to the appropriate
	 *
	 */
	class NGIN_API BaseSink
	{
	public:
		/**
		 * @brief Virtual destructor for interface cleanup.
		 */
		virtual ~BaseSink() = default;

		/**
		 * @brief Initializes the log sink.
		 *
		 * @return true if initialization was successful, false otherwise.
		 */
		virtual bool Init()
		{
			LogHeader();
			return true;
		}

		/**
		 * @brief Cleans up resources associated with the log sink.
		 */
		virtual void Cleanup() {}

		/**
		 * @brief Processes and logs a message to the specified sink.
		 *
		 * @param message The log entry to be processed.
		 */
		virtual void Log(Entry message) = 0;

		/**
		 * @brief Flushes the sink and ensures all pending messages are outputted.
		 */
		virtual void Flush() = 0;

		void LogHeader()
		{
			Log({Level::Info,
				 "Timestamp           | File:Line                  | Level    | Message\n--------------------+----------------------------+----------+"});
		}
	};

} // namespace NGIN
