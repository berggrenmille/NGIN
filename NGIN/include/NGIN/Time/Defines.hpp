#pragma once
#include <chrono>

namespace NGIN::Time
{

    /// @brief Represents a clock that is not affected by system time changes.
    using SteadyClock = std::chrono::steady_clock;
    /// @brief Represents a clock that is affected by system time changes.
    using SystemClock = std::chrono::system_clock;
    /// @brief Represents a high-resolution clock. (Can be affected by system time changes)
    using HighResClock = std::chrono::high_resolution_clock;

    /// @brief Represents a time point.
    /// @tparam T The clock type.
    /// @note This is just a shorthand for std::chrono::time_point.
    template<typename T>
    using TimePoint = std::chrono::time_point<T>;
    /// @brief Represents a duration.
    /// @tparam Rep The type of the duration's tick count.
    /// @tparam Period The tick period of the duration.
    /// @note This is just a shorthand for std::chrono::duration.
    template<typename Rep, typename Period>
    using Duration = std::chrono::duration<Rep, Period>;

    using Time = std::time_t;

    /**
     * @brief Represents a duration of nanoseconds.
     */
    using Nanoseconds = std::chrono::nanoseconds;

    /**
     * @brief Represents a duration of microseconds.
     */
    using Microseconds = std::chrono::microseconds;

    /**
     * @brief Represents a duration of milliseconds.
     */
    using Milliseconds = std::chrono::milliseconds;

    /**
     * @brief Represents a duration of seconds.
     */
    using Seconds = std::chrono::seconds;

    /**
     * @brief Represents a duration of minutes.
     */
    using Minutes = std::chrono::minutes;

    /**
     * @brief Represents a duration of hours.
     */
    using Hours = std::chrono::hours;

    /// \brief Represents a duration of days.
    /// \tparam Rep The type of the duration's tick count.
    template<typename Rep = Int>
    using Days = Duration<Rep, std::ratio_multiply<std::ratio<24>, Hours::period>>;

    /**
     * @brief Represents a duration of weeks.
     */
    template<typename Rep = Int>
    using Weeks = Duration<Rep, std::ratio_multiply<std::ratio<7>, typename Days<Rep>::period>>;

    /**
     * @brief Represents a duration of months (approximated to 30 days).
     */
    template<typename Rep = Int>
    using Months = Duration<Rep, std::ratio_multiply<std::ratio<30>, typename Days<Rep>::period>>;

    /**
     * @brief Represents a duration of years (approximated to 365 days).
     */
    template<typename Rep = Int>
    using Years = Duration<Rep, std::ratio_multiply<std::ratio<365>, typename Days<Rep>::period>>;

    template<class T>
    struct IsDuration : std::false_type {};

    template<class Rep, class Period>
    struct IsDuration<std::chrono::duration<Rep, Period>> : std::true_type {};

    template<typename T>
    concept IsDurationV = IsDuration<T>::value;


}