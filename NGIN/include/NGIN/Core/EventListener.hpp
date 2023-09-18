#pragma once

namespace NGIN::Core
{
	template <typename EventType>
	class EventListener
	{
	public:
		/// @brief Function type for listening to events.
		using Listener = std::function<void(EventType &)>;

		/**
		 * @brief Construct an event listener.
		 *
		 * @param listener The function that listens for the event.
		 */
		EventListener() = delete;

		EventListener(const Listener &other)
			: listener(other)
		{
		}

		EventListener(Listener &&other) noexcept
			: listener(std::move(other))
		{
		}

		EventListener(const EventListener &other)
			: listener(other.listener)
		{
		}

		EventListener(EventListener &&other) noexcept
			: listener(std::move(other.listener))
		{
		}

		EventListener &operator=(EventListener &&other)
		{
			listener = std::move(other.listener);
			return *this;
		}

		/**
		 * @brief Call the listener with the event.
		 *
		 * @param event The event to handle.
		 */
		void Invoke(EventType &event) const
		{
			if (listener)
				listener(event);
		}

	private:
		Listener listener; ///< The internal listener function.
	};
} // namespace NGIN::Core