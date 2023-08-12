#pragma once
#include "LogSink.hpp"

#include <fmt/format.h>

#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <string_view>

class BaseLogger
{
public:
	virtual ~BaseLogger() = default;
	// To be implemented by derived loggers.
	virtual void LogInternal(LogLevel level, const std::string& message) = 0;

	template<typename... Args>
	void Log(LogLevel level, std::string_view fmt, const Args&... args, const std::source_location& location = std::source_location::current())
	{
		std::string message = fmt::format(fmt, args...);
		std::ostringstream logStream;
		logStream << location.file_name() << ":" << location.line() << " " << location.function_name() << " - " << message;
		LogInternal(level, logStream.str());
	}

private:
	std::vector<LogSink*> sinks;
};