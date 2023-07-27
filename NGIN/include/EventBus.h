#pragma once
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace NGIN
{
	/// Base class for all event listeners
	class EventListenerBase
	{
	public:
		virtual ~EventListenerBase() = default;
	};

	/// Template for specific event listeners
	template<typename EventType>
	class EventListener : public EventListenerBase
	{
	public:
		/// Alias for the listener function pointer type
		using Listener = void(*)(EventType&);

		/// Constructs an event listener
		/// @param listener Pointer to the function that handles the event
		explicit EventListener(Listener listener);

		/// Calls the listener function
		/// @param event Reference to the event object
		void operator()(EventType& event) const;

	private:
		Listener listener_;
	};

	/// Class for event bus
	class EventBus
	{
	public:
		/// Subscribes a listener to a specific event type
		/// @tparam EventType Type of event
		/// @param listener Function pointer to the listener
		template<typename EventType>
		void Subscribe(typename EventListener<EventType>::Listener const& listener);

		/// Publishes an event to all its listeners
		/// @tparam EventType Type of event
		/// @param event Reference to the event object
		template<typename EventType>
		void Publish(EventType& event);

	private:
		/// Map of event listeners indexed by event type
		std::unordered_map<std::type_index, void*> listenersMap_;
	};
}

#include "EventBus.inl"
