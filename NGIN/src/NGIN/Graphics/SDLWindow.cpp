//
// Created by Maximiliam Berggren on 04/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
// NGIN Include(s)
#include <NGIN/Graphics/SDLWindow.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Graphics
{

    SDLWindow::~SDLWindow()
    {
        ShutdownImpl();
    }

    Bool SDLWindow::Init(WindowConfig& windowConfig)
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

    void* SDLWindow::GetNativeHandle() const
    {
        return static_cast<void*>(sdlWindow);
    }

    void SDLWindow::GetDimensions(int& outWidth, int& outHeight) const
    {
        SDL_GetWindowSize(sdlWindow, &outWidth, &outHeight);
    }

    void SDLWindow::Shutdown()
    {
        ShutdownImpl();
    }

    void SDLWindow::ShutdownImpl()
    {
        if (!sdlWindow)
            return;
    }
}