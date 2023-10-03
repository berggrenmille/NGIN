#pragma once

#include <NGIN/Defines.hpp>

#include "BaseSink.hpp"
#include <NGIN/Util/Format.hpp>
#include <NGIN/Util/TupleUtils.hpp>
#include <NGIN/Util/StringUtils.hpp>


#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <chrono>

namespace NGIN::Logging
{

    class BaseLogger
    {
    public:
        /// \brief Default constructor.
        NGIN_API BaseLogger() = default;
        /// \brief Virtual destructor.
        NGIN_API virtual ~BaseLogger() = default;

        /// \brief Deleted copy constructor.
        /// Prevents copying of logger instances.
        BaseLogger(const BaseLogger&) = delete;

        /// \brief Deleted copy assignment operator.
        /// Prevents assignment of logger instances.
        BaseLogger& operator=(const BaseLogger&) = delete;

        /// \brief Log a message with formatting.
        /// \details This function takes in its formatting arguments in a tuple in order to be able to support
        ///          having the source location be defaulted to the current source location.
        /// \tparam Args Types of the format arguments.
        /// \param level Log level.
        /// \param message Log message.
        /// \param formatArgs Tuple containing format arguments.
        /// \param location Source location of the log (file, line number, etc).
        template<typename... Args>
        void Log(Level level,
                 const String& message,
                 std::tuple<Args...> formatArgs,
                 const std::source_location& location = std::source_location::current());

        /// \brief Log a simple message without formatting.
        /// \param level Log level.
        /// \param message Log message.
        /// \param location Source location of the log (file, line number, etc).
        void Log(Level level, const String& message,
                 const std::source_location& location = std::source_location::current());

        /// \brief Add a sink to the logger.
        /// \tparam T Type of the sink to add.
        /// \tparam Args Types of the arguments for constructing the sink.
        /// \param args Arguments for constructing the sink.
        /// \return Reference to the added sink.
        template<typename T, typename... Args>
        T& AddSink(Args&& ...args);

    protected:
        /// \brief Internal log function to be implemented by derived classes.
        /// \param level Log level.
        /// \param message Formatted log message.
        NGIN_API virtual void LogInternal(Level level, const String& message) = 0;

        /// \brief Formats the log message.
        /// \param level Log level.
        /// \param message Log message.
        /// \param location Source location of the log caller (file, line number, etc).
        /// \return Formatted log message as a string.
        NGIN_API virtual String FormatLogMessage(Level level,
                                                 const String& message,
                                                 const std::source_location& location);

        /// \brief Collection of sinks that the logger will write to.
        std::vector<Scope<BaseSink>> sinks;
    };

    template<typename... Args>
    void BaseLogger::Log(Level level,
                         const String& message,
                         std::tuple<Args...> formatArgs,
                         const std::source_location& location)
    {
        String messageFmt;
        Util::Apply(formatArgs, [&](auto&& ...args)
        {
            messageFmt = Util::RuntimeFormat(message, std::forward<decltype(args)>(args)...);
        });
        LogInternal(level, FormatLogMessage(level, messageFmt, location));
    }

    template<typename T, typename... Args>
    T& BaseLogger::AddSink(Args&& ...args)
    {
        // Construct the sink with the given arguments
        auto sink = CreateScope<T>(std::forward<Args>(args)...);
        // Get a reference before moving it into the vector
        T& ref = *sink;
        // Move it to the sinks vector
        sinks.push_back(std::move(sink));
        return ref;
    }
}
