//
// Created by Maximiliam Berggren on 01/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
#include <NGIN/Core/Modules/GraphicsModule.hpp>
#include <NGIN/Core/Platform/SDL/SDLModule.hpp>

#include <NGIN/Core/Engine.hpp>

// NGIN Include(s)

// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{


    void GraphicsModule::OnInit(Engine* enginePtr)
    {
        engine = enginePtr;

        Graphics::WindowConfig settings {};
        settings.borderless = false;
        settings.fullscreen = false;
        settings.resizable = true;
        settings.width = 1280;
        settings.height = 720;
        settings.title = "NGIN";


        this->context = new Graphics::Context();
        context->Init(Graphics::GraphicsAPI::VULKAN, &settings);
    }


}