//
// Created by Maximiliam Berggren on 01/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Core/Module.hpp>

#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Graphics/Context.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Core::Modules
{
    class GraphicsModule : public Module
    {
    public:
        NGIN_API static void Dependencies(Engine* engine);

    protected:
        NGIN_API void OnInit(Engine* engine) override;


    private:
        Engine* engine = nullptr;
        Graphics::Window* window = nullptr;
        Graphics::Context* context = nullptr;
    };
}


