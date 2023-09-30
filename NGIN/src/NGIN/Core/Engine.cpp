#include <NGIN/Core/Engine.hpp>
#include <iostream>

namespace NGIN::Core
{
    void Engine::Tick()
    {
        eventBus.Subscribe<Events::Quit>(this, &Engine::Quit);
        isRunning = true;
        F64 delta = 0.0;
        while (!shouldQuit)
        {
            timer.Reset();
            for (auto& module: moduleVector)
            {
                module->OnPreTick(delta);
            }
            for (auto& module: moduleVector)
            {
                module->OnTick(delta);
            }
            for (auto& module: moduleVector)
            {
                module->OnPostTick(delta);
            }
            delta = timer.ElapsedSeconds();
        }
        for (auto& module: moduleVector)
        {
            module->OnShutdown();
        }
        isRunning = false;
    }

    EventBus& Engine::GetEventBus()
    {
        return eventBus;
    }


    void Engine::Quit()
    {
        shouldQuit = true;
    }

    void Engine::Quit(const Events::Quit& event)
    {
        shouldQuit = true;
    }
}