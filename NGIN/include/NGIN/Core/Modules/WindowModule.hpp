#pragma once 
#include <NGIN/Defines.hpp>
#include <NGIN/Core/Module.hpp>
namespace NGIN::Core
{
    class WindowModule : public Module
    {
    public:
        NGIN_API WindowModule() = default;
        NGIN_API ~WindowModule() = default;
        NGIN_API void OnInit(Engine* engine) override;
        NGIN_API void OnPreTick(F64 deltaTime) override;
        NGIN_API void OnTick(F64 deltaTime) override;
        NGIN_API void OnPostTick(F64 deltaTime) override;
        NGIN_API void OnShutdown() override;
    private:
    }
}