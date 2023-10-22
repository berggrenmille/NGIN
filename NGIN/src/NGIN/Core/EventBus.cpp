//
// Created by Maximiliam Berggren on 22/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
// NGIN Include(s)
#include <NGIN/Core/EventBus.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{

    void EventBus::Unsubscribe(const ListenerHandle& handle)
    {
        auto& listeners = listenersMap[handle.eventID];
        listeners.erase(listeners.begin() + static_cast<UInt>(handle.listenerIndex));
    }
}