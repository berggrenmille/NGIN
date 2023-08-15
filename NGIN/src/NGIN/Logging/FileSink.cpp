#include <Precompiled/PCH.h>
#include <NGIN/Logging/FileSink.hpp>


namespace NGIN::Logging
{



	FileSink::FileSink(const std::string& filename) : filename(filename)
	{}

	FileSink::~FileSink()
	{
		Cleanup();
	}

	bool FileSink::Init()
	{
		logFile.open(filename, std::ios::app);
		LogHeader();
		return logFile.is_open();
	}

	void FileSink::Cleanup()
	{
		if (logFile.is_open())
			logFile.flush();
		logFile.close();
	}

	void FileSink::Log(Entry message)
	{
		if (logFile.is_open())
			logFile << message.message << "\n";
	}

	void FileSink::Flush()
	{
		if (logFile.is_open())
			logFile.flush();
	}

} // namespace NGIN::Logging
