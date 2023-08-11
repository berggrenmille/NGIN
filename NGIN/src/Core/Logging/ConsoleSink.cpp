#include "ConsoleSink.hpp"

NGIN::ConsoleSink::ConsoleSink()
{
#ifdef _WIN32
	hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void NGIN::ConsoleSink::logMessage(LogLevel level, const std::string& message)
{
#ifdef _WIN32
	// Windows-specific coloring using Console API
	if (level == LogLevel::WARNING)
	{
		SetConsoleTextAttribute(hHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << message << "\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); // Bright White
	}
	else if (level == LogLevel::ERROR || level == LogLevel::FATAL)
	{
		SetConsoleTextAttribute(hHandle, FOREGROUND_RED | FOREGROUND_INTENSITY); // Bright Red
		std::cerr << message << "\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); // Bright White
	}
	else
	{
		std::cout << message << "\n";
	}
#else
	// ANSI escape code coloring for UNIX-like systems
	if (level == LogLevel::Warning)
	{
		std::cout << "\033[93m" << message << "\033[0m" << "\n";
		return;
	}

	if (level == LogLevel::Error || level == LogLevel::Critical)
	{
		std::cout << "\033[91m" << message << "\033[0m" << "\n";
		return;
	}

	std::cout << message << "\n";

#endif
}
