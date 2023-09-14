#pragma once

#include "EventListener.hpp"

#include <NGIN/Meta/StoragePolicy.hpp>
#include <NGIN/Meta/TypeID.hpp>
#include <vector>
#include <unordered_map>
#include <deque>

namespace NGIN::Core
{

	class EventBus
	{
	public:
		using StoragePolicy = Meta::StoragePolicy::Hybrid<64>;
		using EventVector = std::vector<StoragePolicy>;
		/**
		 * @brief Subscribe to an event.
		 *
		 * @tparam EventType The type of event.
		 * @param listener The function that listens for the event.
		 */
		template <typename EventType>
		void Subscribe(typename EventListener<EventType>::Listener listener)
		{
			auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();

			auto& listeners = listenersMap[eventTypeIndex];
			listeners.emplace_back(EventListener<EventType>(listener));
		}

		/**
		 * @brief Subscribe a member function to an event.
		 *
		 * @tparam EventType The type of event.
		 * @tparam T The type of the instance's class.
		 * @param instance Pointer to the instance.
		 * @param memberFunction Pointer to the member function.
		 */
		template <typename EventType, typename T>
		void Subscribe(T* instance, void (T::* memberFunction)(EventType&))
		{
			auto listener = [instance, memberFunction](EventType& event)
			{
				(instance->*memberFunction)(event);
			};
			auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();

			// auto test = EventListener<EventType>::Listener(listener);
			auto& listeners = listenersMap[eventTypeIndex];

			listeners.emplace_back(EventListener<EventType>(listener));
		}

		/**
		 * @brief Publish (dispatch) an event.
		 *
		 * @tparam EventType The type of event.
		 * @param event The event to dispatch.
		 */
		template <typename EventType>
		void Publish(EventType& event)
		{
			auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();
			if (listenersMap.count(eventTypeIndex) != 0)
			{
				auto& listeners = listenersMap[eventTypeIndex];
				for (auto& eventStorage : listeners)
				{
					auto listener = static_cast<EventListener<EventType> *>(eventStorage.get());
					listener->Invoke(event);
				}
			}
		}

		/**
		 * @brief Destructor. Clears all listeners.
		 */
		~EventBus()
		{
			// for (auto &pair : listenersMap)
			//	{
			// auto listeners = static_cast<EventVectorPtr>(pair.second);

			// delete listeners;
			//}
		}

	private:
		/// @brief Map of event listeners indexed by event type.
		std::unordered_map<size_t, EventVector> listenersMap;
	};

}