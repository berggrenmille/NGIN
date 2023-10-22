#pragma once

#include <NGIN/Defines.hpp>
#include <chrono>
#include "Defines.hpp"
#include "Util.hpp"

namespace NGIN::Time
{
    /// @class Timer
    /// @brief A templated Timer class.
    /// @tparam ClockType The clock type to use for the timer. Ex: NGIN::Time::SteadyClock
    /// @note The timer is started when it is constructed.
    template<typename ClockType>
    class Timer
    {
    public:

        /// @brief Resets the timer to the current time.
        inline void Reset()
        {
            startTime = ClockType::now();
        }

        /// @brief Gets the elapsed time since the timer was started or reset.
        /// @tparam ReturnT The type to return the elapsed time as.
        /// @tparam DurationT The duration type to return the elapsed time as.
        template<typename ReturnT = Int, typename DurationT = Seconds>
        [[nodiscard]] inline ReturnT Elapsed() const
        {
            const auto elapsedTime = ClockType::now() - startTime;
            const auto elapsedF64  = Duration<F64, typename DurationT::period>(elapsedTime).count();
            return static_cast<ReturnT>(elapsedF64);
        }

        /// @brief Gets the elapsed time since the timer was started or reset.
        /// @tparam DurationT The duration type to return the elapsed time as.
        /// @return The elapsed time as a floating point number.
        template<typename DurationT = Seconds>
        [[nodiscard]] inline F64 ElapsedF64() const
        {
            return Elapsed<F64, DurationT>();
        }

        /// @brief Gets the elapsed time since the timer was started or reset.
        /// @tparam DurationT The duration type to return the elapsed time as.
        /// @return The elapsed time as an integer.
        template<typename DurationT = Seconds>
        [[nodiscard]] inline Int ElapsedInt() const
        {
            return Elapsed<Int, DurationT>();
        }

    private:
        TimePoint<ClockType> startTime = ClockType::now();  ///< The start time of the timer.
    };


} // namespace NGIN::Time
