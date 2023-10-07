//
// Created by Maximiliam Berggren on 01/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Core/Module.hpp>
#include <NGIN/Core/Engine.hpp>

#include <NGIN/Graphics/Context.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{
    class GraphicsModule : public Module
    {
    public:


    protected:
        NGIN_API void OnInit(Engine* engine) override;


    private:
        Engine* engine = nullptr;
        Scope <Graphics::Context> context = nullptr;
    };
}


