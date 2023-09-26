#pragma once

#include "Defines.hpp"

namespace NGIN::Time
{
    /**
     * @brief Gets the current time from a steady clock.
     *
     * @return Current time as per std::chrono::steady_clock.
     */
    inline auto Now()
    {
        return std::chrono::steady_clock::now();
    }

    /**
     * @brief Gets the time since the system clock's epoch.
     *
     * @return Duration since the epoch as per std::chrono::system_clock.
     */
    inline auto SinceEpoch()
    {
        return std::chrono::system_clock::now().time_since_epoch();
    }


    template <class T>
    struct is_duration_t : std::false_type
    {};

    template <class Rep, class Period>
    struct is_duration_t<std::chrono::duration<Rep, Period>> : std::true_type
    {};

    template <typename T>
    concept is_duration = is_duration_t<T>::value;

    /**
     * @brief Wraps a long chrono call into a much prettier one
     */
    inline std::time_t GetCurrentTime()
    {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

} // namespace NGIN::Time