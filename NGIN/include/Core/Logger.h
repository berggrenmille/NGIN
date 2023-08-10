#pragma once
#include <Core/Core.h>
#include <HideWarnings/Loguru.hpp>
#include <source_location>
#include <cstdarg>
#include <tuple>
#include <utility>

#include <string>
#include <Detail/LoggingHelper.hpp>
#include <fmt/format.h>


//DEFINES
#define CURR_FILE() ::std::source_location::current()
#define NGIN_LOG(verbosity, ...) ::NGIN::Logger::Log(CURR_FILE(), verbosity, __VA_ARGS__)
#define NGIN_LOG_IF(verbosity, condition, ...) ::NGIN::Logger::LogIf(CURR_FILE(), verbosity, condition, __VA_ARGS__)
#define NGIN_INFO(...) ::NGIN::Logger::Log(CURR_FILE(), ::NGIN::Logger::Verbosity::INFO, __VA_ARGS__)
#define NGIN_WARNING(...) ::NGIN::Logger::Log(CURR_FILE(), ::NGIN::Logger::Verbosity::WARNING, __VA_ARGS__)
#define NGIN_ERROR(...) ::NGIN::Logger::Log(CURR_FILE(), ::NGIN::Logger::Verbosity::ERROR, __VA_ARGS__)

#ifdef NGIN_TESTING 
#define NGIN_ASSERT(condition, ...) (condition) == true ? (void)0 : std::abort()
#define NGIN_ASSERT_SRC(condition, source, ...) (condition) == true ? (void)0 : std::abort();
#else
#define NGIN_ASSERT(condition, ...) (condition) == true ? (void)0 : loguru::log_and_abort(0, "CHECK FAILED: " #condition "  ", __FILE__, __LINE__, ##__VA_ARGS__)
#define NGIN_ASSERT_SRC(condition, source, ...) (condition) == true ? (void)0 : loguru::log_and_abort(0, "CHECK FAILED: " #condition "  ", source.file_name(), source.line(), ##__VA_ARGS__)
#endif
namespace NGIN
{
	namespace Logger
	{
		/**
		 * @brief Represents the verbosity levels of the logging system.
		 *
		 * Basically a wrapper for Logurus verbosity levels
		 */
		enum class Verbosity : int
		{
			// Used to mark an invalid verbosity. Do not log to this level.
			INVALID = -10, // Never do LOG_F(INVALID)

			// You may use Verbosity_OFF on g_stderr_verbosity, but for nothing else!
			OFF = -9, // Never do LOG_F(OFF)

			// Prefer to use ABORT_F or ABORT_S over LOG_F(FATAL) or LOG_S(FATAL).
			FATAL = -3,
			ERROR = -2,
			WARNING = -1,

			// Normal messages. By default written to stderr.
			INFO = 0,

			// Same as Verbosity_INFO in every way.
			Verbosity_0 = 0,

			// Verbosity levels 1-9 are generally not written to stderr, but are written to file.
			Verbosity_1 = +1,
			Verbosity_2 = +2,
			Verbosity_3 = +3,
			Verbosity_4 = +4,
			Verbosity_5 = +5,
			Verbosity_6 = +6,
			Verbosity_7 = +7,
			Verbosity_8 = +8,
			Verbosity_9 = +9,

			// Do not use higher verbosity levels, as that will make grepping log files harder.
			Verbosity_MAX = +9,
		};

		/*
		 * @brief Initializes the logging system.
		 *
		 * This function should be called before any logging is done.
		 *
		 * @param argc The number of command line arguments.
		 * @param argv The command line arguments.
		 */
		inline static void Init(int& argc, char* argv[])
		{
			loguru::g_preamble_thread = false;
			loguru::init(argc, argv);

			loguru::add_file("NGIN.log", loguru::Truncate, loguru::Verbosity_INFO);

		}

		inline void setupLoggingForTests()
		{
			loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

		}

		inline void cleanupLoggingForTests()
		{

			loguru::g_stderr_verbosity = 0;
		}


		template <class... Args>
		inline static void Log_impl(std::tuple<Args...> args, Verbosity verbosity, const std::source_location& location)
		{

			Detail::Apply(args, [&](auto&&... args) {
				loguru::log(static_cast<int>(verbosity), location.file_name(), location.line(), std::forward<decltype(args)>(args)...);
						  });
		}



		template <class... Args>
		inline static void Log(const std::source_location& location = std::source_location::current(), Verbosity verbosity = Verbosity::INFO, const char* msg = "", Args... args)
		{
			Log_impl(Detail::FORMAT_TO_TUPLE(msg, args...), verbosity, location);
		}

	};
}





