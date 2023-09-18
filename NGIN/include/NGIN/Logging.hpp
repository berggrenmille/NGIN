#pragma once
#include <NGIN/Logging/AsyncLogger.hpp>
#include <NGIN/Logging/ConsoleSink.hpp>
#include <NGIN/Logging/FileSink.hpp>
#include <NGIN/Logging/Defines.hpp>
#include <thread>

namespace NGIN::Logging
{
    NGIN_API BaseLogger &GetEngineLogger();
    NGIN_API BaseLogger &GetClientLogger();

    NGIN_API void Init();
    NGIN_API void Shutdown();
}

#define NGIN_LOG_SRC(src, level, msg, ...) \
    ::NGIN::Logging::GetEngineLogger().Log(level, msg, ::NGIN::Util::MultArgs(__VA_ARGS__), src)
#define NGIN_LOG(level, msg, ...) \
    ::NGIN::Logging::GetEngineLogger().Log(level, msg, ::NGIN::Util::MultArgs(__VA_ARGS__))
#define NGIN_INFO(msg, ...) \
    ::NGIN::Logging::GetEngineLogger().Log(::NGIN::Logging::Level::Info, msg, ::NGIN::Util::MultArgs(__VA_ARGS__))
#define NGIN_WARNING(msg, ...) \
    ::NGIN::Logging::GetEngineLogger().Log(::NGIN::Logging::Level::Warning, msg, ::NGIN::Util::MultArgs(__VA_ARGS__))
#define NGIN_ERROR(msg, ...) \
    ::NGIN::Logging::GetEngineLogger().Log(::NGIN::Logging::Level::Error, msg, ::NGIN::Util::MultArgs(__VA_ARGS__))

#define NGIN_ASSERT(condition, msg, ...)                                                                                        \
    do                                                                                                                          \
    {                                                                                                                           \
        if (!(condition))                                                                                                       \
        {                                                                                                                       \
            ::NGIN::Logging::GetEngineLogger().Log(::NGIN::Logging::Level::Critical, msg, ::NGIN::Util::MultArgs(__VA_ARGS__)); \
            std::abort();                                                                                                       \
        }                                                                                                                       \
    } while (0)

#define NGIN_ASSERT_SRC(condition, src, msg, ...)                                                                                    \
    do                                                                                                                               \
    {                                                                                                                                \
        if (!(condition))                                                                                                            \
        {                                                                                                                            \
            ::NGIN::Logging::GetEngineLogger().Log(::NGIN::Logging::Level::Critical, msg, ::NGIN::Util::MultArgs(__VA_ARGS__), src); \
            std::abort();                                                                                                            \
        }                                                                                                                            \
    } while (0)
