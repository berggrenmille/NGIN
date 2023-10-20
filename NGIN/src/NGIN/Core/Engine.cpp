#include <NGIN/Core/Engine.hpp>
#include <iostream>
#include <NGIN/Logging.hpp>

namespace NGIN::Core
{
    void Engine::Run()
    {
        if (!isInitialized)
        {
            NGIN_ERROR("Engine not initialized");
            return;
        }

        isRunning = true;
        F64 delta = 0.0;
        while (!shouldQuit)
        {
            timer.Reset();
            Tick(delta);
            delta = timer.Elapsed<F64, Time::Seconds>();
        }

        //Shutdown modules in reverse order
        for (auto it = moduleVector.rbegin(); it != moduleVector.rend(); ++it)
        {
            (*it)->OnShutdown();
        }
        isRunning = false;
    }

    void Engine::Tick(F64 delta)
    {
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

    Bool Engine::Init()
    {
        eventBus.Subscribe<Events::Quit>(this, &Engine::Quit);
        isInitialized = true;
        NGIN_INFO("Engine initialized");
        return true;
    }


}