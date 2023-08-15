#include <NGIN/Logging.hpp>


namespace
{
	NGIN::Logging::AsyncLogger* engineLogger;
	NGIN::Logging::AsyncLogger* clientLogger;
}

namespace NGIN::Logging
{
	void Init()
	{
		engineLogger = new AsyncLogger(std::chrono::milliseconds(16));
		engineLogger->AddSink<FileSink>("NGIN.log").Init();
		engineLogger->AddSink<ConsoleSink>().Init();
		clientLogger = new AsyncLogger(std::chrono::milliseconds(16));
		engineLogger->AddSink<FileSink>("Client.log").Init();

	}

	void Shutdown()
	{
		delete engineLogger;
		delete clientLogger;
	}

	BaseLogger& GetEngineLogger()
	{
		return *engineLogger;
	}
	BaseLogger& GetClientLogger()
	{
		return *clientLogger;
	}
}

