#pragma once

namespace NGIN::Core
{
	template <typename EventType>
	class EventListener
	{
	public:
		/// @brief Function type for listening to events.
		using Listener = std::function<void(EventType&)>;

		/**
		 * @brief Construct an event listener.
		 *
		 * @param listener The function that listens for the event.
		 */

		EventListener(Listener listener)
			: listener(listener)
		{}

		EventListener(const EventListener& other)
		{
			listener = other.listener;
		}


		/**
		 * @brief Call the listener with the event.
		 *
		 * @param event The event to handle.
		 */
		void Invoke(EventType& event) const
		{
			listener(event);
		}

	private:
		Listener listener; ///< The internal listener function.
	};
} // namespace NGIN::Core