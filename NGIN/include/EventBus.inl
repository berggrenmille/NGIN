#pragma once
#include "EventBus.h"

namespace NGIN
{

	template<typename EventType>
	EventListener<EventType>::EventListener(Listener listener) : listener_(listener) {}

	template<typename EventType>
	void EventListener<EventType>::operator()(EventType& event) const
	{
		listener_(event);
	}

	template<typename EventType>
	void EventBus::Subscribe(typename EventListener<EventType>::Listener const& listener)
	{
		auto eventTypeIndex = std::type_index(typeid(EventType));
		if (listenersMap_.count(eventTypeIndex) == 0)
		{
			listenersMap_[eventTypeIndex] = new std::vector<EventListenerBase*>;
		}

		auto listeners = static_cast<std::vector<EventListenerBase*>*>(listenersMap_[eventTypeIndex]);
		listeners->push_back(new EventListener<EventType>(listener));
	}

	template<typename EventType>
	void EventBus::Publish(EventType& event)
	{
		auto eventTypeIndex = std::type_index(typeid(EventType));
		if (listenersMap_.count(eventTypeIndex) != 0)
		{
			auto listeners = static_cast<std::vector<EventListenerBase*>*>(listenersMap_[eventTypeIndex]);
			for (auto& listener : *listeners)
			{
				(*static_cast<EventListener<EventType>*>(listener))(event);
			}
		}
	}
}
