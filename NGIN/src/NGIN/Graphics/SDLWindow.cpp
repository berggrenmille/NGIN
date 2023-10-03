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
    SDLWindow::SDLWindow(String title, int width, int height)
            : title(std::move(title)), width(width), height(height), sdlWindow(nullptr) {}

    SDLWindow::~SDLWindow()
    {
        ShutdownImpl();
    }

    bool SDLWindow::Init()
    {
        sdlWindow = SDL_CreateWindow(title.c_str(),
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     width,
                                     height,
                                     SDL_WINDOW_OPENGL);
        if (sdlWindow == nullptr)
            return false;

        return true;
    }

    void* SDLWindow::GetNativeHandle() const
    {
        return static_cast<void*>(sdlWindow);
    }

    void SDLWindow::GetDimensions(int& width, int& height) const
    {
        SDL_GetWindowSize(sdlWindow, &width, &height);
    }

    void SDLWindow::Shutdown()
    {
        ShutdownImpl();
    }

    void SDLWindow::ShutdownImpl()
    {
        if (sdlWindow)
        {
            SDL_DestroyWindow(sdlWindow);
            sdlWindow = nullptr;
        }
    }
}