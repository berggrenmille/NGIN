#pragma once
#include <NGIN/Meta/StoragePolicy.hpp>
#include <NGIN/Meta/TypeID.hpp>

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
        explicit EventListener(Listener listener)
            : listener(listener)
        {
        }

        /**
         * @brief Call the listener with the event.
         *
         * @param event The event to handle.
         */
        void Invoke(EventType &event) const
        {
            listener(event);
        }

    private:
        Listener listener; ///< The internal listener function.
    };

    class EventBus
    {
    public:
        using EventVector = std::vector<Meta::StoragePolicy::Dynamic *>;
        using EventVectorPtr = std::vector<Meta::StoragePolicy::Dynamic *> *;
        /**
         * @brief Subscribe to an event.
         *
         * @tparam EventType The type of event.
         * @param listener The function that listens for the event.
         */
        template <typename EventType>
        void Subscribe(const typename EventListener<EventType>::Listener &listener)
        {
            auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();
            if (listenersMap.count(eventTypeIndex) == 0)
            {
                listenersMap[eventTypeIndex] = new EventVector;
            }

            auto listeners = static_cast<EventVectorPtr>(listenersMap[eventTypeIndex]);
            listeners->push_back(new NGIN::Meta::StoragePolicy::Dynamic(EventListener<EventType>(listener)));
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
        void Subscribe(T *instance, void (T::*memberFunction)(EventType &))
        {
            auto listener = [instance, memberFunction](EventType &event)
            {
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
        template <typename EventType>
        void Publish(EventType &event)
        {
            auto eventTypeIndex = NGIN::Meta::TypeID<EventType>();
            if (listenersMap.count(eventTypeIndex) != 0)
            {
                auto listeners = static_cast<EventVectorPtr>(listenersMap[eventTypeIndex]);
                for (auto &listenerBase : *listeners)
                {
                    auto listener = static_cast<EventListener<EventType> *>(listenerBase->get());
                    listener->Invoke(event);
                }
            }
        }

        /**
         * @brief Destructor. Clears all listeners.
         */
        ~EventBus()
        {
            for (auto &pair : listenersMap)
            {
                auto listeners = static_cast<EventVectorPtr>(pair.second);

                delete listeners;
            }
        }

    private:
        /// @brief Map of event listeners indexed by event type.
        std::unordered_map<size_t, EventVectorPtr> listenersMap;
    };

}