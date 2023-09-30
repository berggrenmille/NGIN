//
// Created by Maximiliam Berggren on 01/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
#include <NGIN/Core/Modules/GraphicsModule.hpp>
#include <NGIN/Core/Modules/WindowModule.hpp>

#include <NGIN/Core/Engine.hpp>

// NGIN Include(s)

// STL Include(s)
// Other Include(s)

namespace NGIN::Core::Modules
{

    void Modules::GraphicsModule::Dependencies(Engine* engine)
    {
        engine->AddModule<WindowModule>();
    }

    void GraphicsModule::OnInit(Engine* enginePtr)
    {
        this->engine = enginePtr;
        this->window = new Graphics::Window();

        Graphics::WindowSettings settings {};
        settings.borderless = false;
        settings.fullscreen = false;
        settings.resizable = true;
        settings.width = 1280;
        settings.height = 720;
        settings.title = "NGIN";

        window->Init(Graphics::GraphicsAPI::VULKAN, settings);

        this->context = new Graphics::Context();
        context->Init(Graphics::GraphicsAPI::VULKAN, window);
    }


}