#pragma once

#include <NGIN/Defines.hpp>
#include <chrono>
#include "Defines.hpp"
#include "Util.hpp"

namespace NGIN::Time
{


    /**
     * @class Timer
     * @brief A high-resolution timer used to measure durations.
     *
     * @tparam T The duration type to be used for measurements.
     */

    class Timer
    {
    public:

        /**
         * @brief Resets the timer to the current time.
         */
        void Reset()
        {
            startTime = Now();
        }

        /**
         * @brief Gets the elapsed time since the timer was started or reset.
         *
         * @return Elapsed time as a duration of type T.
         */
        template<is_duration T>
        [[nodiscard]] T Elapsed() const
        {
            return std::chrono::duration_cast<T>(Now() - startTime);
        }

        [[nodiscard]] F64 ElapsedSeconds() const
        {
            return std::chrono::duration<F64>(Elapsed<Microseconds>()).count();
        }

    private:
        std::chrono::steady_clock::time_point startTime = Now();  ///< The start time of the timer.
    };


} // namespace NGIN::Time
