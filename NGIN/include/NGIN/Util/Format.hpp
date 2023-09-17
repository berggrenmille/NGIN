#pragma once

#include <HideWarnings/fmt.hpp>

#include <iostream>
#include <sstream>
#include <string>

namespace NGIN
{
    namespace Util
    {
        /**
         * @brief Performs runtime formatting on a given message string using provided format arguments.
         *
         * This function allows for dynamically formatting a message string based on the given formatArgs tuple.
         * The actual formatting is performed using the fmt library.
         *
         * @tparam Args The types of format arguments contained in the formatArgs tuple.
         * @param message The message string to be formatted.
         * @param formatArgs A tuple containing the arguments to be used for formatting the message.
         * @return A formatted string based on the provided message and format arguments.
         */
        template <typename... Args>
        std::string RuntimeFormat(std::string_view message, Args... args)
        {
            return fmt::format(fmt::runtime(message), std::forward<Args>(args)...);
        }

        /**
         * @brief Formats a string using a format string and arguments.
         *
         * This function wraps around the `fmt::format` function, providing a consistent
         * interface for string formatting within the codebase. It takes a format string
         * and a variable number of arguments to format the string accordingly.
         *
         * @param message The format string.
         * @param args The arguments to be substituted into the format string.
         * @return The formatted string.
         *
         * @note This function only works with string literals. For runtime formatting, use the `NGIN::Util::RuntimeFormat` function.
         *
         * @example Util::Format("Hello, {}", "world!");  // Returns "Hello, world!"
         */
        template <typename... Args>
        std::string Format(fmt::format_string<Args...> fmt, Args &&...args)
        {

            auto str = fmt::format(fmt, std::forward<Args>(args)...);
            return str;
        }
    }
}