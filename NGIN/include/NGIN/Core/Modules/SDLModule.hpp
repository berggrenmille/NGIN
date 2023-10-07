//
// Created by Maximiliam Berggren on 07/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Core/Module.hpp>

#include <NGIN/Core/Engine.hpp>

// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{
    class NGIN_API SDLModule : public Module
    {
    public:

    protected:
        void OnInit(Engine* engine) override;

        void OnShutdown() override;

        void OnPreTick(F64 deltaTime) override;

    private:
        Engine* engine = nullptr;

    };
}


