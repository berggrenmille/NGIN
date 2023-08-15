#pragma once
#include <string>

namespace NGIN
{
	namespace Logging
	{
		/**
		 * @brief Enum class representing the log severity/level.
		 */
		enum class Level
		{
			Trace,    ///< Trace-level log, usually very detailed.
			Info,     ///< Informational messages, general knowledge about system operations.
			Debug,    ///< Used during debugging to log additional info.
			Warning,  ///< Represents warnings that aren't critical but might indicate potential issues.
			Error,    ///< Represents errors that aren't fatal but might impact normal operation.
			Critical  ///< Critical errors that might cause the system to terminate.
		};

		/**
		* @brief Converts a log level to a string.
		*/
		constexpr const char* LogLevelToString(Level level)
		{
			switch (level)
			{
			case Level::Debug:		return "DEBUG";
			case Level::Info:		return "INFO";
			case Level::Warning:	return "WARNING";
			case Level::Error:		return "ERROR";
			case Level::Critical:	return "FATAL";
			default:				return "UNKNOWN";
			}
		}

		/**
		*  @brief Representation for all data a sink needs to print
		*/
		struct Entry
		{
			Level level;
			std::string message;
		};
	}
}