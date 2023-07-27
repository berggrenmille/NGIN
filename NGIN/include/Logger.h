#pragma once
#include <Core.h>
#include <loguru.hpp>
#include <source_location>
#include <cstdarg>
namespace NGIN
{
	//Wrapper for loguru verbosity
	enum Verbosity : int
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
	/// <summary>
	/// LOG is only used to initialize the loguru library
	/// </summary>
	namespace Logger
	{


		inline static void Init(int& argc, char* argv[])
		{
			loguru::init(argc, argv);
		}

		inline static void Log(Verbosity verbosity, const std::source_location& location, const char* format, ...)
		{
			va_list va;
			va_start(va, format);
			loguru::vlog(verbosity, location.file_name(), location.line(), format, va);
		}

		inline static void Log(Verbosity verbosity, const char* format, ...)
		{
			va_list va;
			va_start(va, format);
			loguru::vlog(verbosity, "Unknown", 0, format, va);
		}
	};
}
#define CURR_FILE() ::std::source_location::current()
#define NGIN_LOG(...) ::LOG_F(INFO, __VA_ARGS__)
#define NGIN_WARNING(...) ::LOG_F(WARNING, __VA_ARGS__)
#define NGIN_ERROR(...) ::LOG_F(ERROR, __VA_ARGS__)
#define NGIN_LOG_FUNCTION(verbosity) ::LOG_SCOPE_FUNCTION(verbosity)

