#include <NGIN/Logging/BaseLogger.hpp>
#include <NGIN/Time.hpp>

#include <HideWarnings/fmt.hpp>

namespace NGIN::Logging
{
    String BaseLogger::FormatLogMessage(Level level,
                                        const String& message,
                                        const std::source_location& location)
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

    void BaseLogger::Log(Level level, const String& message, const std::source_location& location)
    {

        LogInternal(level, FormatLogMessage(level, message, location));
    }
}