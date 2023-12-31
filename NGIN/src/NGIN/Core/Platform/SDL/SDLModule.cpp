#include <NGIN/Core/Platform/SDL/SDLModule.hpp>
#include <NGIN/Core/Events/Quit.hpp>
#include <NGIN/Core/Engine.hpp>
#include <NGIN/Core/Events/WindowEvents.hpp>


namespace NGIN::Core
{


    void SDLModule::OnInit(Engine* engine)
    {
        this->engine = engine;
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    }

    void SDLModule::OnPreTick(F64 deltaTime)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    Events::Quit quitEvent {};
                    engine->GetEventBus().Publish(quitEvent);
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    HandleSdlWindowEvent(event);
                    break;
                }

                default:
                    break;
            }
        }
    }

    void SDLModule::OnTick(F64 deltaTime)
    {

    }


    void SDLModule::OnShutdown()
    {
        SDL_Quit();
    }

    void SDLModule::HandleSdlWindowEvent(const SDL_Event& event)
    {
        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
            {
                //window->Resize(event.window.data1, event.window.data2);
                Events::WindowResizeEvent resizeEvent(event.window.data1, event.window.data2);
                engine->GetEventBus().Publish(resizeEvent);
                break;
            }
            case SDL_WINDOWEVENT_CLOSE:
            {
                Events::WindowCloseEvent closeEvent;
                engine->GetEventBus().Publish(closeEvent);
                break;
            }
            case SDL_WINDOWEVENT_MOVED:
            {
                Events::WindowMoveEvent moveEvent(event.window.data1, event.window.data2);
                engine->GetEventBus().Publish(moveEvent);
                break;
            }
            case SDL_WINDOWEVENT_MINIMIZED:
            {
                Events::WindowMinimizeEvent minimizeEvent;
                engine->GetEventBus().Publish(minimizeEvent);
                break;
            }
            case SDL_WINDOWEVENT_MAXIMIZED:
            {
                Events::WindowMaximizeEvent maximizeEvent;
                engine->GetEventBus().Publish(maximizeEvent);
                break;
            }
            case SDL_WINDOWEVENT_RESTORED:
            {
                Events::WindowRestoreEvent restoreEvent;
                engine->GetEventBus().Publish(restoreEvent);
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                Events::WindowFocusEvent focusEvent;
                engine->GetEventBus().Publish(focusEvent);
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                Events::WindowLostFocusEvent lostFocusEvent;
                engine->GetEventBus().Publish(lostFocusEvent);
                break;
            }
            default:
                break;
        }
    }
}
