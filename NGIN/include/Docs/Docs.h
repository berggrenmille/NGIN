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
	namespace Detail {}

	/**
	 * @brief The namespace encapsulates the Logging functions of the NGIN.
	 *
	 * Logge contains basically a wrapper around the loguru library. It provides both a functional interface and macros for logging
	 */
	namespace Logger {}
}