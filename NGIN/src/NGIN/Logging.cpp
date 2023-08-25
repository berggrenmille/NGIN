#include <NGIN/Logging.hpp>

namespace
{
	NGIN::Logging::BaseLogger *engineLogger;
	NGIN::Logging::BaseLogger *clientLogger;
}

namespace NGIN::Logging
{
	void Init()
	{
		engineLogger = new AsyncLogger(std::chrono::milliseconds(16));
		clientLogger = new AsyncLogger(std::chrono::milliseconds(16));

		engineLogger->AddSink<FileSink>("NGIN.log").Init();
		engineLogger->AddSink<ConsoleSink>().Init();
		engineLogger->AddSink<FileSink>("Client.log").Init();
	}

	void Shutdown()
	{
		delete engineLogger;
		delete clientLogger;
	}

	BaseLogger &GetEngineLogger()
	{
		return *engineLogger;
	}
	BaseLogger &GetClientLogger()
	{
		return *clientLogger;
	}
}
