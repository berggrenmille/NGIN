#pragma once
#include <NGIN/Defines.hpp>
#include <NGIN/Util/Delegate.hpp>
#include <NGIN/Meta/TypeID.hpp>
#include <vector>
#include <unordered_map>
#include <deque>

namespace NGIN::Core
{

	class EventBus
	{
	public:
		using EventVector = std::vector<DynamicDelegate>;

		template <typename EventType, typename FuncType>
			requires requires (FuncType t) { DynamicDelegate(std::move(t)); }
		void Subscribe(FuncType&& func)
		{
			auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();

			auto& listeners = listenersMap[eventTypeIndex];
			//auto test = EventListener<EventType>(std::move(listener));

			listeners.emplace_back(std::forward<FuncType>(func));
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
			auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();
			auto& listeners = listenersMap[eventTypeIndex];
			listeners.emplace_back(std::move(DynamicDelegate(memberFunction, instance)));
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
				for (auto& delegate : listeners)
				{
					delegate(event);
				}
			}
		}

		/**
		 * @brief Destructor. Clears all listeners.
		 */
		~EventBus()
		{

		}

	private:
		/// @brief Map of event listeners indexed by event type.
		std::unordered_map<UInt64, EventVector> listenersMap;
	};

}