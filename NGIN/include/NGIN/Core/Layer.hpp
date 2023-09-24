#pragma once
#include <NGIN/Defines.hpp>

namespace NGIN::Core
{
    class Layer
    {
    public:
        Layer();
        virtual ~Layer();


        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}


    protected:

    };
}