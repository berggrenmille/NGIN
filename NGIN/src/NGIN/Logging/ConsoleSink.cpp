#include <NGIN/Logging/ConsoleSink.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>  // for isatty()
#endif
namespace NGIN::Logging
{
	ConsoleSink::ConsoleSink()
	{
#ifdef _WIN32
		hConsole = (void*)GetStdHandle(STD_OUTPUT_HANDLE);
#else
		isTerminal = isatty(fileno(stdout));
#endif
	}


	void ConsoleSink::Log(Entry message)
	{
		SetColor(message.level);
		std::cout << message.message << "\n";
		ResetColor();
	}

	void ConsoleSink::Flush()
	{
		std::cout.flush();
	}

#ifdef _WIN32
	void ConsoleSink::SetColor(Level level)
	{
		WORD color;
		switch (level)
		{
		case Level::Warning:
			color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case Level::Error:
		case Level::Critical:
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
	void ConsoleSink::SetColor(Level level)
	{
		if (!isTerminal) return;

		const char* colorCode;
		switch (level)
		{
		case Level::Warning:
			colorCode = "\033[93m";  // Bright Yellow
			break;
		case Level::Error:
		case Level::Critical:
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
}
