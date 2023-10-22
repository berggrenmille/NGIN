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

        /// @brief Default constructor
        EventBus() = default;

        /// @brief Default constructor
        ~EventBus() = default;

        template<typename EventType, typename FuncType>
        requires std::is_same_v<typename Meta::FunctionTraits<FuncType>::template ArgNType<0>, const EventType&>
        ListenerHandle Subscribe(FuncType&& func);


        /**
         * @brief Subscribe a member function to an event.
         *
         * @tparam EventType The type of event.
         * @tparam T The type of the instance's class.
         * @param instance Pointer to the instance.
         * @param memberFunction Pointer to the member function.
         */
        template<typename EventType, typename T>
        ListenerHandle Subscribe(T* instance, void (T::* memberFunction)(const EventType&));


        void Unsubscribe(const ListenerHandle& handle);


        /**
         * @brief Publish (dispatch) an event.
         *  This will call all listeners subscribed to the event type.
         *  The event will be passed to the listeners.
         * @tparam EventType The type of event.
         * @param event The event to dispatch.
         */
        template<typename EventType>
        void Publish(const EventType& event);


    private:

        /// @brief Map of event listeners indexed by event type.
        std::unordered_map<UInt64, EventVector> listenersMap;
    };


    // Inline template implementations
    template<typename EventType, typename FuncType>
    requires std::is_same_v<typename Meta::FunctionTraits<FuncType>::template ArgNType<0>, const EventType&>
    inline ListenerHandle EventBus::Subscribe(FuncType&& func)
    {
        auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();

        auto& listeners = listenersMap[eventTypeIndex];
        //auto test = EventListener<EventType>(std::move(listener));

        listeners.emplace_back(std::forward<FuncType>(func));
        return {eventTypeIndex, listeners.size() - 1};
    }

    template<typename EventType, typename T>
    inline ListenerHandle EventBus::Subscribe(T* instance, void (T::* memberFunction)(const EventType&))
    {
        constexpr auto eventTypeIndex = Meta::TypeID<EventType>();
        auto& listeners = listenersMap[eventTypeIndex];
        listeners.emplace_back(std::move(DynamicDelegate(memberFunction, instance)));
        return {eventTypeIndex, listeners.size() - 1};
    }

    template<typename EventType>
    inline void EventBus::Publish(const EventType& event)
    {
        constexpr auto eventTypeIndex = Meta::TypeID<EventType>(); //Get the type index of the event

        if (listenersMap.count(eventTypeIndex) == 0)        //If there are no listeners for this event type
            return;

        auto& listeners = listenersMap[eventTypeIndex];            // Get the listeners for this event type

        for (auto& delegate: listeners)
            delegate(event);

    }
}