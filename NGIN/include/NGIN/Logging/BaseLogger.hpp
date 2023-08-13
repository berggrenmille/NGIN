#pragma once
#include "BaseSink.hpp"
#include <Utils/TupleHelper.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/TimeUtils.hpp>

#include <fmt/format.h>


#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <chrono>
#include <fmt/chrono.h>
namespace NGIN::Logging
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
		void Log(Level level, const std::string& message, std::tuple<Args...> formatArgs = std::make_tuple(), const std::source_location& location = std::source_location::current())
		{
			//Format message 
			std::string messageFmt = Util::RuntimeFormat(message, formatArgs);

			//Get other information
			auto file = Util::ExtractFileName(location.file_name());
			auto timeNow = Util::GetCurrentTime();

			//format log output
			std::string finalMessage = Util::Format("{:%Y-%m-%d %H:%M:%S} | {:>20}:{:<5} | {:<8} | {}",
													fmt::localtime(timeNow),
													file,
													location.line(),
													LogLevelToString(level),
													messageFmt);
			//Send formated message to implementation
			LogInternal(level, finalMessage);
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
		virtual void LogInternal(Level level, const std::string& message) = 0;

		std::vector<std::unique_ptr<BaseSink>> sinks;


	};
}
