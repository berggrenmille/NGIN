#include <NGIN/Core/Modules/SDLModule.hpp>

#include <NGIN/Core/Events/Quit.hpp>
#include <NGIN/Core/Engine.hpp>
#include <NGIN/Core/Events/WindowEvents.hpp>

#include <SDL2/SDL.h>


namespace NGIN::Core
{

    static inline void HandleSdlWindowEvent(Engine* engine, const SDL_Event& event)
    {
        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
            {
                //window->Resize(event.window.data1, event.window.data2);
                Events::WindowResize resizeEvent(event.window.data1, event.window.data2);
                engine->GetEventBus().Publish(resizeEvent);
                break;
            }
            case SDL_WINDOWEVENT_CLOSE:
            {
                Events::WindowClose closeEvent;
                engine->GetEventBus().Publish(closeEvent);
                break;
            }
            case SDL_WINDOWEVENT_MOVED:
            {
                Events::WindowMove moveEvent(event.window.data1, event.window.data2);
                engine->GetEventBus().Publish(moveEvent);
                break;
            }
            case SDL_WINDOWEVENT_MINIMIZED:
            {
                Events::WindowMinimize minimizeEvent;
                engine->GetEventBus().Publish(minimizeEvent);
                break;
            }
            case SDL_WINDOWEVENT_MAXIMIZED:
            {
                Events::WindowMaximize maximizeEvent;
                engine->GetEventBus().Publish(maximizeEvent);
                break;
            }
            case SDL_WINDOWEVENT_RESTORED:
            {
                Events::WindowRestore restoreEvent;
                engine->GetEventBus().Publish(restoreEvent);
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                Events::WindowFocus focusEvent;
                engine->GetEventBus().Publish(focusEvent);
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                Events::WindowLostFocus lostFocusEvent;
                engine->GetEventBus().Publish(lostFocusEvent);
                break;
            }
            default:
                break;
        }
    }


    void SDLModule::OnInit(Engine* engine)
    {
        this->engine = engine;
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    }

    void SDLModule::OnPreTick(const F64 deltaTime)
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
                    HandleSdlWindowEvent(engine, event);
                    break;
                }

                default:
                    break;
            }
        }
    }

    void SDLModule::OnShutdown()
    {
        SDL_Quit();
    }


}
