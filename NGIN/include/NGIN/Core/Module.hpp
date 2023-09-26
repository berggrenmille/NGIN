#pragma once
#include <NGIN/Defines.hpp>

namespace NGIN::Core
{
    class Module
    {
    public:
        Module() = default;
        virtual ~Module() = default;


        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnTick(const F64 deltaTime) {}


    };
}