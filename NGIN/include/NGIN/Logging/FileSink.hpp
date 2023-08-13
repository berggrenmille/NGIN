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
	class NGIN_API FileSink : public BaseSink
	{
	public:
		/**
		 * @brief Constructs a new FileSink object, specifying the file to write to.
		 *
		 * @param filename The name of the file to which log entries will be written.
		 */
		FileSink(const std::string& filename);

		/**
		 * @brief Default destructor. Ensures that the log file is closed.
		 */
		virtual ~FileSink();

		/**
		 * @brief Initializes the log sink by opening the file for writing.
		 *
		 * @return true if initialization was successful (file opened successfully), false otherwise.
		 */
		virtual bool Init() override;

		/**
		 * @brief Cleans up resources associated with the log sink. Closes the log file.
		 */
		virtual void Cleanup() override;

		/**
		 * @brief Writes a log entry to the file.
		 *
		 * @param entry The log entry to write.
		 */
		virtual void Log(Entry message) override;

		/**
		 * @brief Flushes any buffered log entries to the file.
		 */
		virtual void Flush() override;

	private:
		std::string filename;
		std::ofstream logFile;
	};

} // namespace NGIN::Logging
