#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Core
{
    class Engine;

    class Module
    {
    public:
        friend class Engine;

        Module() = default;

        virtual ~Module() = default;

    protected:
        NGIN_API virtual void OnInit(Engine* engine) {}

        NGIN_API virtual void OnShutdown() {}

        NGIN_API virtual void OnPreTick(const F64 deltaTime) {}

        NGIN_API virtual void OnTick(const F64 deltaTime) {}

        NGIN_API virtual void OnPostTick(const F64 deltaTime) {}


    };
}