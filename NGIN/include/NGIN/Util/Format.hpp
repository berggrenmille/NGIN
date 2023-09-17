#pragma once

#include <HideWarnings/fmt.hpp>

namespace NGIN::Util
{
    /// @brief Formats a string at runtime using provided format arguments.
    /// This function wraps around the `fmt::format` function from the fmt library.
    ///
    /// @note For compile-time string formatting, see `NGIN::Util::Format` function.
    ///
    /// @tparam Args Types of the arguments used for formatting.
    /// @param message The message string to be formatted.
    /// @param args The arguments to be substituted into the format string.
    /// @return The formatted string.
    ///
    /// @code
    ///   std::string_view msg = "Hello, {}!";
    ///   std::string result = RuntimeFormat(msg, "World");
    ///   // result will be "Hello, World!"
    /// @endcode
    template <typename... Args>
    std::string RuntimeFormat(std::string_view message, Args &&...args)
    {
        return fmt::format(fmt::runtime(message), std::forward<Args>(args)...);
    }

    /// @brief Formats a string at compile-time using provided format arguments.
    /// This function wraps around the `fmt::format` function from the fmt library.
    ///
    /// @note For runtime string formatting, see `NGIN::Util::RuntimeFormat` function.
    ///
    /// @tparam Args Types of the arguments used for formatting.
    /// @param message The format string (must be a string literal).
    /// @param args The arguments to be substituted into the format string.
    /// @return The formatted string.
    /// @code
    ///   constexpr const char* msg = "Hello, {}!";
    ///   std::string result = Format(msg, "World");
    ///   // result will be "Hello, World!"
    /// @endcode
    template <typename... Args>
    constexpr std::string Format(fmt::format_string<Args...> message, Args &&...args)
    {
        return fmt::format(message, std::forward<Args>(args)...);
    }

}
