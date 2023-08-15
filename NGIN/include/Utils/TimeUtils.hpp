#pragma once
#include <chrono>

namespace NGIN
{
	namespace Util
	{
		/**
		  * @brief Wraps a long chrono call into a much prettier one
		  */
		inline std::time_t GetCurrentTime()
		{
			return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}
	}
}