// Include necessary libraries
#include <unordered_map>
#include <functional>
#include <vector>
#include <typeindex>

class EventBus
{
public:
	// Define an alias for a Listener function that takes an Event as parameter
	template<typename EventType>
	using Listener = std::function<void(EventType&)>;

	// Subscribe function adds a new listener for a particular Event type
	template<typename EventType>
	void Subscribe(Listener<EventType> const& listener)
	{
		// Get reference to the list of listeners for this Event type
		auto& listeners = GetListeners<EventType>();
		// Add the listener to the list
		listeners.push_back(listener);
	}

	// Publish function dispatches an Event to all its listeners
	template<typename EventType>
	void Publish(EventType& event)
	{
		// Get reference to the list of listeners for this Event type
		auto& listeners = GetListeners<EventType>();
		// Pass the event to each listener
		for (auto& listener : listeners)
		{
			listener(event);
		}
	}

private:
	// GetListeners function fetches a list of listeners for a particular Event type
	template<typename EventType>
	std::vector<Listener<EventType>>& GetListeners()
	{
		// Get type id of the Event
		const std::type_index typeId = std::type_index(typeid(EventType));
		// Find the listeners list for this type id in the map
		auto it = listenersMap.find(typeId);

		// If there's no listeners list for this type id yet
		if (it == listenersMap.end())
		{
			// Create a new listeners list and add it to the map
			it = listenersMap.emplace(typeId, new std::vector<Listener<EventType>>()).first;
		}

		// Return the listeners list for this type id
		return *(std::vector<Listener<EventType>>*)it->second;
	}

	// Map to hold lists of listeners for each Event type
	std::unordered_map<std::type_index, void*> listenersMap;
};