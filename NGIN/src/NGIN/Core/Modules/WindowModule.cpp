#include <NGIN/Core/Modules/WindowModule.hpp>
#include <NGIN/Core/Events/Quit.hpp>
#include <NGIN/Core/Engine.hpp>
namespace NGIN::Core
{

    Graphics::Window* WindowModule::GetWindow() const
    {
        return window;
    }

    void WindowModule::OnInit(Engine* engine)
    {
        this->engine = engine;
        window = new Graphics::Window();
        Graphics::WindowSettings windowSettings;
        windowSettings.title = "NGIN";
        windowSettings.width = 1280;
        windowSettings.height = 720;
        window->Init(Graphics::GraphicsAPI::VULKAN, windowSettings);
    }

    void WindowModule::OnPreTick(F64 deltaTime)
    {}

    void WindowModule::OnTick(F64 deltaTime)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                auto quitEvent = Events::Quit();
                engine->GetEventBus().Publish(quitEvent);
                break;

            default:
                break;
            }
        }
    }

    void WindowModule::OnPostTick(F64 deltaTime)
    {}

    void WindowModule::OnShutdown()
    {
        window->Shutdown();
        delete window;
    }
}
