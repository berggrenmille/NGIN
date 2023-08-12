#include "ConsoleSink.hpp"
namespace NGIN
{
	ConsoleSink::ConsoleSink()
	{
#ifdef _WIN32
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else
		isTerminal = isatty(fileno(stdout));
#endif
	}


	void ConsoleSink::Log(LogLevel level, const std::string& message)
	{
		setColor(level);
		std::cerr << message << "\n";
		resetColor();
	}

#ifdef _WIN32
	void ConsoleSink::SetColor(LogLevel level)
	{
		WORD color;
		switch (level)
		{
		case LogLevel::WARNING:
			color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case LogLevel::ERROR:
		case LogLevel::FATAL:
			color = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		default:
			color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		}
		SetConsoleTextAttribute(hConsole, color);
	}

	void ConsoleSink::ResetColor()
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}

#else  // POSIX
	void ConsoleSink::SetColor(LogLevel level)
	{
		if (!isTerminal) return;

		const char* colorCode;
		switch (level)
		{
		case LogLevel::WARNING:
			colorCode = "\033[93m";  // Bright Yellow
			break;
		case LogLevel::ERROR:
		case LogLevel::FATAL:
			colorCode = "\033[91m";  // Bright Red
			break;
		default:
			colorCode = "\033[97m";  // Bright White
		}
		std::cout << colorCode;
	}

	void ConsoleSink::ResetColor()
	{
		if (isTerminal)
			std::cout << "\033[0m";  // Reset
	}
#endif
