#pragma once

#include <NGIN/Defines.hpp>
#include <chrono>
#include "Defines.hpp"
#include "Util.hpp"

namespace NGIN::Time
{
    /// @class Timer
    /// @brief A high-resolution timer used to measure durations.
    ///
    /// @tparam T The duration type to be used for measurements.
    template<typename ClockType>
    class Timer
    {
    public:

        /// @brief Resets the timer to the current time.
        void Reset()
        {
            startTime = ClockType::now();
        }

        /// @brief Gets the elapsed time since the timer was started or reset.
        /// @return Elapsed time as a duration of type T.
        template<IsDurationV T>
        [[nodiscard]] T Elapsed() const
        {
            return std::chrono::duration_cast<T>(Now() - startTime);
        }

        [[nodiscard]] F64 ElapsedSeconds() const
        {
            return std::chrono::duration<F64>(Elapsed<Microseconds>()).count();
        }

    private:
        TimePoint<ClockType> startTime = ClockType::now();  ///< The start time of the timer.
    };


} // namespace NGIN::Time
