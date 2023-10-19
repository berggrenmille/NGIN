#pragma once
#include "BaseSink.hpp"
#include <iostream>
#include <string>


namespace NGIN::Logging
{
    /**
     * @class ConsoleSink
     * @brief A log sink that prints log messages to the standard output or error.
     *
     * Logs messages to the standard output or error based on their severity level,
     * and sets the output color based on the severity level.
     */
    class NGIN_API ConsoleSink : public BaseSink
    {
    public:
        ConsoleSink();

        /**
         * @brief Logs a message to the standard output or error.
         *
         * @param level The severity level of the message.
         * @param message The message to be logged.
         */
        void Log(Entry message) override;

        void Flush() override;

    private:
#ifdef _WIN32
        void* hConsole;
#else
        bool isTerminal;
#endif

        /**
         * @brief Sets the output color based on the severity level.
         *
         * @param level The severity level of the message.
         */
        void SetColor(Level level);

        /**
         * @brief Resets the output color to default.
         */
        void ResetColor();
    };

} // namespace NGIN