//
// Created by Maximiliam Berggren on 04/10/2023.
//

//Precompiled include
#include "Precompiled/PCH.h"
// NGIN Include(s)
#include "NGIN/Graphics/Window.hpp"

#include <SDL2/SDL.h>


namespace NGIN::Graphics
{

    Window::~Window()
    {
        Shutdown();
    }

    Bool Window::Init(WindowConfig& windowConfig)
    {
        UInt32 flags = 0;
        if (windowConfig.fullscreen)
            flags |= SDL_WINDOW_FULLSCREEN;
        if (windowConfig.resizable)
            flags |= SDL_WINDOW_RESIZABLE;
        if (windowConfig.borderless)
            flags |= SDL_WINDOW_BORDERLESS;

        switch (windowConfig.api)
        {
            case GraphicsAPI::VULKAN:
                flags |= SDL_WINDOW_VULKAN;
                break;
            case GraphicsAPI::OPENGL:
                flags |= SDL_WINDOW_OPENGL;
                break;
            case GraphicsAPI::DX12:
            default:
                break;
        }

        sdlWindow = SDL_CreateWindow(windowConfig.title.c_str(),
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     windowConfig.width,
                                     windowConfig.height,
                                     flags);
        return sdlWindow != nullptr;
    }


    void Window::GetDimensions(int& outWidth, int& outHeight) const
    {
        SDL_GetWindowSize(sdlWindow, &outWidth, &outHeight);
    }

    void Window::Shutdown()
    {
        if (!sdlWindow)
            return;
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
}


