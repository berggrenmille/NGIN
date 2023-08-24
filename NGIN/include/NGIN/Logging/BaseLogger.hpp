#pragma once
#include "BaseSink.hpp"
#include <NGIN/Util/TupleHelper.hpp>
#include <NGIN/Util/StringUtils.hpp>
#include <NGIN/Util/TimeUtils.hpp>

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
		NGIN_API BaseLogger() = default;
		NGIN_API virtual ~BaseLogger() = default;
		// Delete copy operations
		NGIN_API BaseLogger(const BaseLogger &) = delete;
		NGIN_API BaseLogger &operator=(const BaseLogger &) = delete;

		template <typename... Args>
		void Log(Level level, const std::string &message, std::tuple<Args...> formatArgs, const std::source_location &location = std::source_location::current())
		{
			// Format message
			std::string messageFmt = Util::RuntimeFormat(message, formatArgs);
			LogInternal(level, FormatLogMessage(level, messageFmt, location));
		}

		template <typename... Args>
		void Log(Level level, const std::string &message, const std::source_location &location = std::source_location::current())
		{

			LogInternal(level, FormatLogMessage(level, message, location));
		}

		template <typename T, typename... Args>
		T &AddSink(Args &&...args)
		{
			// Construct the sink with the given arguments
			auto sink = std::make_unique<T>(std::forward<Args>(args)...);

			// Get a reference before moving it into the vector
			T &ref = *sink;

			// Move it to the sinks vector
			sinks.push_back(std::move(sink));

			// Return reference to the user
			return ref;
		}

	protected:
		// To be implemented by derived loggers.
		NGIN_API virtual void LogInternal(Level level, const std::string &message) = 0;
		NGIN_API virtual std::string FormatLogMessage(Level level, const std::string &message, const std::source_location &location);

		std::vector<std::unique_ptr<BaseSink>> sinks;
	};
	/*
	template<>
	inline void BaseLogger::Log(Level level, const std::string& message, std::tuple<> formatArgs, const std::source_location& location)
	{
		Log(level, message, location);
	}
	*/
}
