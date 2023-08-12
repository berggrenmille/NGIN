#pragma once
#include "LogSink.hpp"
#include <iostream>
#include <string>

#ifdef _WIN32 | _WIN64
#include <Windows.h>
#else
#include <unistd.h>  // for isatty()
#endif

namespace NGIN
{

	class ConsoleSink : public LogSink
	{
	public:
		ConsoleSink();
		virtual void Log(LogLevel level, const std::string& message) override;

	private:
#ifdef _WIN32
		HANDLE hConsole;
#else
		bool isTerminal;
#endif
		void SetColor(LogLevel level);
		void ResetColor();
	};

} // namespace NGIN