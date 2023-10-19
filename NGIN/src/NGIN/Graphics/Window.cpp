//
// Created by Maximiliam Berggren on 04/10/2023.
//

//Precompiled include
#include "Precompiled/PCH.h"
// NGIN Include(s)
#include "NGIN/Graphics/Window.hpp"
#include <NGIN/Logging.hpp>

#include <SDL2/SDL.h>


namespace NGIN::Graphics
{

    Window::~Window()
    {
        Shutdown();
    }

    Bool Window::Init(const WindowConfig& windowConfig)
    {
        UInt32 flags = 0;
        flags |= windowConfig.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
        flags |= windowConfig.resizable ? SDL_WINDOW_RESIZABLE : 0;
        flags |= windowConfig.borderless ? SDL_WINDOW_BORDERLESS : 0;

        switch (windowConfig.api)
        {
            case GraphicsAPI::VULKAN:
                flags |= SDL_WINDOW_VULKAN;
                break;
            case GraphicsAPI::OPENGL:
                flags |= SDL_WINDOW_OPENGL;
                [[fallthrough]];
            case GraphicsAPI::DX12:
                [[fallthrough]];
            default:
                NGIN_ASSERT(false, "GraphicsAPI not supported!");
                break;
        }

        sdlWindow = SDL_CreateWindow(windowConfig.title.c_str(),
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     static_cast<Int32>(windowConfig.width),
                                     static_cast<Int32>(windowConfig.height),
                                     flags);

        if (!sdlWindow)
        {
            NGIN_ERROR("Failed to create window: {}", SDL_GetError());
            return false;
        }

        return true;
    }


    void Window::GetDimensions(UInt32& outWidth, UInt32& outHeight) const
    {
        int x, y;
        SDL_GetWindowSize(sdlWindow, &x, &y);
        outWidth  = static_cast<UInt32>(x);
        outHeight = static_cast<UInt32>(y);
    }

    void Window::Shutdown()
    {
        if (!sdlWindow)
        {
            NGIN_WARNING("Trying to shutdown a window that is not initialized! UUID: {}",
                         uuid.ToString());
            return;
        }

        SDL_DestroyWindow(sdlWindow);
        sdlWindow = nullptr;
        NGIN_INFO("Window with UUID: {} has been shutdown!", uuid.ToString());
    }

    void Window::Resize(UInt32 width, UInt32 height)
    {
        SDL_SetWindowSize(sdlWindow, static_cast<Int32>(width), static_cast<Int32>(height));
    }

    Bool Window::IsOpen() const
    {
        return sdlWindow != nullptr;
    }

    SDL_Window* Window::GetSDLWindow() const
    {
        return sdlWindow;
    }

    void* Window::GetNativeHandle() const
    {
        return sdlWindow;
    }
}


