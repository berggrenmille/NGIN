#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Util/Delegate.hpp>
#include <NGIN/Meta/TypeID.hpp>
#include <NGIN/Meta/FunctionTraits.hpp>
#include <vector>
#include <type_traits>
#include <unordered_map>
#include <deque>

namespace NGIN::Core
{
    /// @brief The mode in which an event will be dispatched.
    enum class EventMode : UInt8
    {
        /// @brief The event will be dispatched immediately.
        Immediate,
        /// @brief The event will be dispatched at the end of the frame.
        Queued
    };

    struct ListenerHandle
    {
        UInt64 eventID;
        UInt64 listenerIndex;
    };


    class EventBus
    {
    public:
        using EventVector = std::vector<DynamicDelegate>;
        using QueuedEvent = StaticDelegate<void()>;

        template<typename EventType, typename FuncType>
        requires std::is_same_v<typename Meta::FunctionTraits<FuncType>::template ArgNType<0>, const EventType&>
        ListenerHandle Subscribe(FuncType&& func)
        {
            auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();

            auto& listeners = listenersMap[eventTypeIndex];
            //auto test = EventListener<EventType>(std::move(listener));

            listeners.emplace_back(std::forward<FuncType>(func));
            return {eventTypeIndex, listeners.size() - 1};
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
        ListenerHandle Subscribe(T* instance, void (T::* memberFunction)(const EventType&))
        {
            constexpr auto eventTypeIndex = Meta::TypeID<EventType>();
            auto& listeners = listenersMap[eventTypeIndex];
            listeners.emplace_back(std::move(DynamicDelegate(memberFunction, instance)));
            return {eventTypeIndex, listeners.size() - 1};
        }

        void Unsubscribe(const ListenerHandle& handle)
        {
            auto& listeners = listenersMap[handle.eventID];
            listeners.erase(listeners.begin() + static_cast<UInt>(handle.listenerIndex));
        }

        /**
         * @brief Publish (dispatch) an event.
         *  This will call all listeners subscribed to the event type.
         *  The event will be passed to the listeners.
         * @tparam EventType The type of event.
         * @param event The event to dispatch.
         */
        template<typename EventType>
        void Publish(EventType event, const EventMode mode = EventMode::Immediate)
        {
            constexpr auto eventTypeIndex = Meta::TypeID<EventType>();

            const EventType& constEvent = event;
            if (listenersMap.count(eventTypeIndex) != 0)
            {
                auto& listeners = listenersMap[eventTypeIndex];
                for (auto& delegate: listeners)
                {
                    if (mode == EventMode::Immediate)
                        delegate(constEvent);
                    else
                        eventQueue.push_back([=]() mutable { delegate(constEvent); });
                }
            }
        }

        void FlushEvents()
        {
            while (!eventQueue.empty())
            {
                eventQueue.front()();   //Call the delegate
                eventQueue.pop_front(); //Remove the delegate
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
        /// @brief Queue for events to be processed later.
        std::deque<QueuedEvent> eventQueue;
    };

}