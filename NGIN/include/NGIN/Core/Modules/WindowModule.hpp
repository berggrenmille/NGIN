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
        NGIN_API void OnAttach() override;
        NGIN_API void Tick(F64 deltaTime) override;
        NGIN_API void OnDetach() override;
    private:
    }
}