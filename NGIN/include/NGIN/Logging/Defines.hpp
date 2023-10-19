#pragma once
#include <NGIN/Defines.hpp>
namespace NGIN::Logging
{

    /// \enum Level
    /// \brief Enum class representing the log severity/level.
    enum class Level
    {
        Trace,     ///< Trace-level log, usually very detailed.
        Info,      ///< Informational messages, general knowledge about system operations.
        Debug,     ///< Used during debugging to log additional info.
        Warning,   ///< Represents warnings that aren't critical but might indicate potential issues.
        Error,     ///< Represents errors that aren't fatal but might impact normal operation.
        Critical,  ///< Critical errors that might cause the system to terminate.
        CUSTOM0,   ///< Custom log level 0.
        CUSTOM1,   ///< Custom log level 1.
        CUSTOM2,   ///< Custom log level 2.
        CUSTOM3,   ///< Custom log level 3.
        CUSTOM4,   ///< Custom log level 4.
        CUSTOM5,   ///< Custom log level 5.
        CUSTOM6,   ///< Custom log level 6.
        CUSTOM7,   ///< Custom log level 7.
        CUSTOM8,   ///< Custom log level 8.
        CUSTOM9,   ///< Custom log level 9.
        CUSTOM10,  ///< Custom log level 10.
        CUSTOM11,  ///< Custom log level 11.
        CUSTOM12,  ///< Custom log level 12.
        CUSTOM13,  ///< Custom log level 13.
        CUSTOM14,  ///< Custom log level 14.
        CUSTOM15,  ///< Custom log level 15.
        CUSTOM16,  ///< Custom log level 16.
        CUSTOM17,  ///< Custom log level 17.
        CUSTOM18,  ///< Custom log level 18.
        CUSTOM19,  ///< Custom log level 19.
    };

    /// \brief Converts a log level to a string.
    constexpr const char* LogLevelToString(Level level)
    {
        switch (level)
        {
            case Level::Trace:
                return "TRACE";
            case Level::Debug:
                return "DEBUG";
            case Level::Info:
                return "INFO";
            case Level::Warning:
                return "WARNING";
            case Level::Error:
                return "ERROR";
            case Level::Critical:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }

    /**
    *  @brief Representation for all data a sink needs to print
    */
    struct Entry
    {
        Level  level;
        String message;
        const SourceLocation& src;
    };
}
