#pragma once

/**
 * @brief The main namespace
 */
namespace NGIN
{

	/**
	 * @brief A namespace containing internal helper functions
	 *
	 *  Contains very specialized code, so use externally at own risk.
	 */
	namespace Util {}

	/**
	 * @brief The namespace encapsulates the Logging functions of the NGIN.
	 *
	 * Logger contains basically a wrapper around the loguru library. It provides both a functional interface and macros for logging
	 *
	 * @deprecated Will be removed and be replaced by @see Logging
	 */
	namespace Logger {}

	/**
	 * @brief Provides functionalities related to the logging process.
	 *
	 * This namespace encompasses all the utilities, handlers, formatters, and other components
	 * that are essential for the logging process within the system. It offers a holistic approach
	 * to capture, format, and manage log entries for various applications.
	 */
	namespace Logging {}
}