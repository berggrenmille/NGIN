#pragma once
#include <chrono>

namespace NGIN::Time
{
    using Nanoseconds = std::chrono::nanoseconds;
    using Microseconds = std::chrono::microseconds;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
    using Hours = std::chrono::hours;
    using Days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, Hours::period>>;
    using Weeks = std::chrono::duration<int, std::ratio_multiply<std::ratio<7>, Days::period>>;
    using Months = std::chrono::duration<int, std::ratio_multiply<std::ratio<30>, Days::period>>;
    using Years = std::chrono::duration<int, std::ratio_multiply<std::ratio<365>, Days::period>>;
}