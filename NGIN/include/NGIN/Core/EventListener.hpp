#pragma once
#include <NGIN/Defines.hpp>
#include <NGIN/Util/Delegate.hpp>
namespace NGIN::Core
{
	template <typename EventType>
	class EventListener
	{
	public:
		/// @brief Function type for listening to events.
		using Listener = StaticDelegate<void(EventType&)>;

		/**
		 * @brief Construct an event listener.
		 *
		 * @param listener The function that listens for the event.
		 */
		EventListener() = delete;


		EventListener(Listener&& other) noexcept
			: listener(std::move(other))
		{}



		EventListener(EventListener&& other) noexcept
			: listener(std::move(other.listener))
		{}

		EventListener& operator=(EventListener&& other) noexcept
		{
			listener = std::move(other.listener);
			return *this;
		}

		/**
		 * @brief Call the listener with the event.
		 *
		 * @param event The event to handle.
		 */
		void Invoke(EventType& event)
		{
			if (listener)
				listener(event);
		}

	private:
		Listener listener; ///< The internal listener function.
	};
} // namespace NGIN::Core