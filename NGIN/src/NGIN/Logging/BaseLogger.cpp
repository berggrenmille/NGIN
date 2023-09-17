#include <NGIN/Logging/BaseLogger.hpp>
#include <NGIN/Time.hpp>

#include <HideWarnings/fmt.hpp>
namespace NGIN::Logging
{
	std::string BaseLogger::FormatLogMessage(Level level, const std::string &message, const std::source_location &location)
	{
		auto file = Util::ExtractFileName(location.file_name());
		auto timeNow = Time::GetCurrentTime();

		return Util::Format("{:%Y-%m-%d %H:%M:%S} | {:>20}:{:<5} | {:<8} | {}",
							fmt::localtime(timeNow),
							file,
							location.line(),
							LogLevelToString(level),
							message);
	}
}