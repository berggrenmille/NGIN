#pragma once

#include "BaseSink.hpp"
#include <fstream>

namespace NGIN::Logging
{

	/**
	 * @class FileSink
	 * @brief A log sink that writes log entries to a file.
	 *
	 * The FileSink handles writing log messages to a specified file. It provides
	 * cross-platform file logging capabilities using the C++ Standard Library.
	 */
	class FileSink : public BaseSink
	{
	public:
		/**
		 * @brief Constructs a new FileSink object, specifying the file to write to.
		 *
		 * @param filename The name of the file to which log entries will be written.
		 */
		NGIN_API FileSink(const std::string& filename);

		/**
		 * @brief Default destructor. Ensures that the log file is closed.
		 */
		NGIN_API virtual ~FileSink();

		/**
		 * @brief Initializes the log sink by opening the file for writing.
		 *
		 * @return true if initialization was successful (file opened successfully), false otherwise.
		 */
		NGIN_API virtual bool Init() override;

		/**
		 * @brief Cleans up resources associated with the log sink. Closes the log file.
		 */
		NGIN_API virtual void Cleanup() override;

		/**
		 * @brief Writes a log entry to the file.
		 *
		 * @param entry The log entry to write.
		 */
		NGIN_API virtual void Log(Entry message) override;

		/**
		 * @brief Flushes any buffered log entries to the file.
		 */
		NGIN_API virtual void Flush() override;

	private:
		std::string filename;
		std::ofstream logFile;
	};

} // namespace NGIN::Logging
