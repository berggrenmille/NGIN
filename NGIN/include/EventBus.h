#pragma once
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace NGIN
{
	/**
	 * @brief Base class for all event listeners.
	 */
	class EventListenerBase {};

	/**
	 * @brief Specific event listener template.
	 *
	 * @tparam EventType Type of the event.
	 */
	template<typename EventType>
	class EventListener : public EventListenerBase
	{
	public:
		/**
		 * @brief Alias for the listener function pointer type.
		 */
		using Listener = void(*)(EventType&);

		/**
		 * @brief Constructs an event listener.
		 *
		 * @param listener Pointer to the function that handles the event.
		 */
		explicit EventListener(Listener listener);

		/**
		 * @brief Calls the listener function.
		 *
		 * @param event Reference to the event object.
		 */
		void operator()(EventType& event) const;

	private:
		/**
		 * @brief Listener function pointer.
		 */
		Listener listener;
	};

	/**
	 * @brief Class for event bus.
	 */
	class EventBus
	{
	public:
		/**
		 * @brief Subscribes a listener to a specific event type.
		 *
		 * @tparam EventType Type of event.
		 * @param listener Function pointer to the listener.
		 */
		template<typename EventType>
		void Subscribe(typename EventListener<EventType>::Listener const& listener);

		/**
		 * @brief Publishes an event to all its listeners.
		 *
		 * @tparam EventType Type of event.
		 * @param event Reference to the event object.
		 */
		template<typename EventType>
		void Publish(EventType& event);

	private:
		// Map of event listeners indexed by event type
		std::unordered_map<std::type_index, void*> listenersMap;
	};
}

#include "EventBus.inl"
