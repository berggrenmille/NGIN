#pragma once
#include "LogSink.hpp"
#include <Utils/TupleHelper.hpp>

#include <fmt/format.h>


#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <chrono>
#include <fmt/chrono.h>
namespace NGIN
{
	class NGIN_API BaseLogger
	{
	public:
		BaseLogger() = default;
		virtual ~BaseLogger() = default;
		// Delete copy operations
		BaseLogger(const BaseLogger&) = delete;
		BaseLogger& operator=(const BaseLogger&) = delete;

		template <typename... Args>
		void Log(LogLevel level, std::string_view message, std::tuple<Args...> formatArgs = std::make_tuple(), const std::source_location& location = std::source_location::current())
		{
			auto now = std::chrono::system_clock::now();
			std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
			std::string test;
			Util::Apply(formatArgs, [&](auto&&... formatArgs) {
				test = std::move(fmt::format(fmt::runtime(message), std::forward<decltype(formatArgs)>(formatArgs)...));
						});



			const char* file = strrchr(location.file_name(), '/') + 1;

			auto args = fmt::make_format_args(fmt::localtime(timeNow), file, location.line(), LogLevelToString(level), test);

			std::string formattedMessage = fmt::vformat("{:%Y-%m-%d %H:%M:%S} | {:>20}:{:<5} | {:<8} | {}", args);

			LogInternal(level, formattedMessage);
		}

		template<typename T, typename... Args>
		T& AddSink(Args&&... args)
		{
			// Construct the sink with the given arguments
			auto sink = std::make_unique<T>(std::forward<Args>(args)...);

			// Get a reference before moving it into the vector
			T& ref = *sink;

			// Move it to the sinks vector
			sinks.push_back(std::move(sink));

			// Return reference to the user
			return ref;
		}


	protected:
		// To be implemented by derived loggers.
		virtual void LogInternal(LogLevel level, const std::string& message) = 0;
		std::vector<std::unique_ptr<LogSink>> sinks;

		constexpr const char* LogLevelToString(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Debug: return "DEBUG";
			case LogLevel::Info: return "INFO";
			case LogLevel::Warning: return "WARNING";
			case LogLevel::Error: return "ERROR";
			case LogLevel::Critical: return "FATAL";
			default: return "UNKNOWN";
			}
		}


	};
}
