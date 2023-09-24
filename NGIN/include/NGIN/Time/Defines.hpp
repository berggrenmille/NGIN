#pragma once
#include <chrono>

namespace NGIN::Time
{

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

    /**
     * @brief Represents a duration of days.
     */
    using Days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, Hours::period>>;

    /**
     * @brief Represents a duration of weeks.
     */
    using Weeks = std::chrono::duration<int, std::ratio_multiply<std::ratio<7>, Days::period>>;

    /**
     * @brief Represents a duration of months (approximated to 30 days).
     */
    using Months = std::chrono::duration<int, std::ratio_multiply<std::ratio<30>, Days::period>>;

    /**
     * @brief Represents a duration of years (approximated to 365 days).
     */
    using Years = std::chrono::duration<int, std::ratio_multiply<std::ratio<365>, Days::period>>;
}