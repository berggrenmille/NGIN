#include <iostream>
#include <string>
#include "ILogSink.hpp"
#include "LogLevel.hpp"
#ifdef _WIN32
#include <Windows.h>
#endif

namespace NGIN
{

	class ConsoleSink : public ILogSink
	{
	public:
		ConsoleSink();
		virtual void logMessage(LogLevel level, const std::string& message) override;
	private:
#ifdef _WIN32
		HANDLE hConsole;
#endif
	};

} // namespace NGIN