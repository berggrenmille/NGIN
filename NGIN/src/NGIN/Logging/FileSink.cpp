#include <NGIN/Logging/FileSink.hpp>

namespace NGIN::Logging
{

	FileSink::FileSink(const std::string& filename)
		: filename(filename)
	{}

	FileSink::~FileSink()
	{
		Cleanup();
	}

	bool FileSink::Init()
	{
		logFile.open(filename, std::ios::app);
		return logFile.is_open();
	}

	void FileSink::Cleanup()
	{
		if (logFile.is_open())
		{
			logFile.close();
		}
	}

	void FileSink::Log(Entry message)
	{
		if (logFile.is_open())
		{
			logFile << message.message << std::endl;
		}
	}

	void FileSink::Flush()
	{
		if (logFile.is_open())
		{
			logFile.flush();
		}
	}

} // namespace NGIN::Logging
