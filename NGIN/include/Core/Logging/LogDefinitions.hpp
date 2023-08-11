#pragma once

namespace NGIN
{

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

	// ... other related definitions or utility functions ...

}  // namespace NGIN
