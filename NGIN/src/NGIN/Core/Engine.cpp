#include <NGIN/Core/Engine.hpp>

namespace NGIN::Core
{
    void Engine::Tick()
    {
        F64 delta = timer.ElapsedSeconds();
        timer.Reset();
        for (auto& module : moduleVector)
        {
            module->OnTick(delta);
        }
    }
}