#pragma once
#include <NGIN/Defines.hpp>

namespace NGIN::Core
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;


        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}


    protected:

    };
}