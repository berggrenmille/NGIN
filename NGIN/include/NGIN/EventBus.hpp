#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace NGIN
{
	/// @brief Base class for event listeners.
	class EventListenerBase {};

	/**
	 * @brief EventListener for specific events.
	 *
	 * @tparam EventType The type of event.
	 */
	template<typename EventType>
	class EventListener : public EventListenerBase
	{
	public:
		/// @brief Function type for listening to events.
		using Listener = std::function<void(EventType&)>;


		/**
		 * @brief Construct an event listener.
		 *
		 * @param listener The function that listens for the event.
		 */
		explicit EventListener(Listener listener)
			: listener(listener)
		{}

		/**
		 * @brief Call the listener with the event.
		 *
		 * @param event The event to handle.
		 */
		void operator()(EventType& event) const
		{
			listener(event);
		}

	private:
		Listener listener;///< The internal listener function.
	};

	/**
	 * @brief EventBus class for managing event listeners and dispatching events.
	 */
	class EventBus
	{
	public:
		/**
		 * @brief Subscribe to an event.
		 *
		 * @tparam EventType The type of event.
		 * @param listener The function that listens for the event.
		 */
		template<typename EventType>
		void Subscribe(const typename EventListener<EventType>::Listener& listener)
		{
			auto eventTypeIndex = std::type_index(typeid(EventType));
			if (listenersMap.count(eventTypeIndex) == 0)
			{
				listenersMap[eventTypeIndex] = new std::vector<EventListenerBase*>;
			}

			auto listeners = static_cast<std::vector<EventListenerBase*>*>(listenersMap[eventTypeIndex]);
			listeners->push_back(new EventListener<EventType>(listener));
		}

		/**
		 * @brief Subscribe a member function to an event.
		 *
		 * @tparam EventType The type of event.
		 * @tparam T The type of the instance's class.
		 * @param instance Pointer to the instance.
		 * @param memberFunction Pointer to the member function.
		 */
		template<typename EventType, typename T>
		void Subscribe(T* instance, void (T::* memberFunction)(EventType&))
		{
			auto listener = [instance, memberFunction](EventType& event) {
				(instance->*memberFunction)(event);
			};
			Subscribe<EventType>(listener);
		}

		/**
		 * @brief Publish (dispatch) an event.
		 *
		 * @tparam EventType The type of event.
		 * @param event The event to dispatch.
		 */
		template<typename EventType>
		void Publish(EventType& event)
		{
			auto eventTypeIndex = std::type_index(typeid(EventType));
			if (listenersMap.count(eventTypeIndex) != 0)
			{
				auto listeners = static_cast<std::vector<EventListenerBase*>*>(listenersMap[eventTypeIndex]);
				for (auto& listenerBase : *listeners)
				{
					auto listener = static_cast<EventListener<EventType>*>(listenerBase);
					(*listener)(event);
				}
			}
		}

		/**
		 * @brief Destructor. Clears all listeners.
		 */
		~EventBus()
		{
			for (auto& pair : listenersMap)
			{
				auto listeners = static_cast<std::vector<EventListenerBase*>*>(pair.second);
				for (auto& listener : *listeners)
				{
					delete listener;
				}
				delete listeners;
			}
		}

	private:
		/// @brief Map of event listeners indexed by event type.
		std::unordered_map<std::type_index, std::vector<EventListenerBase*>*> listenersMap;
	};

}
