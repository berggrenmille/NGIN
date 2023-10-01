#pragma once

#include <NGIN/Defines.hpp>

#include <NGIN/Meta/TypeWrapper.hpp>


namespace NGIN::Core
{
    class Engine;

    template<typename... Ts>
    struct DependencyWrapper {};


    class Module
    {
    public:
        friend class Engine;

        using Dependencies = Meta::TypeWrapper<void>;

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