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


	///templates to check if a type is a duration
	template<class T>
	struct is_duration_t : std::false_type {};

	template<class Rep, class Period>
	struct is_duration_t<std::chrono::duration<Rep, Period>> : std::true_type {};

	template <typename T>
	concept is_duration = is_duration_t<T>::value;

	/**
	 * @class Timer
	 * @brief A high-resolution timer used to measure durations.
	 *
	 * @tparam T The duration type to be used for measurements.
	 */
	template<is_duration T>
	class Timer
	{
	public:
		/**
		 * @brief Constructs a new Timer and starts it.
		 */
		Timer() : startTime(Now()) {}

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
		T Elapsed() const
		{
			return std::chrono::duration_cast<T>(Now() - startTime);
		}

	private:
		std::chrono::steady_clock::time_point startTime;  ///< The start time of the timer.
	};

} // namespace NGIN::Time