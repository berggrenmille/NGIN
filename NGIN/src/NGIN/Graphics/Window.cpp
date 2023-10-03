#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Logging.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{
    bool Window::Init(WindowSettings& settings)
    {
        if (isInitialized)
            return true;
        UInt32 flags = 0; // Add base flags here

        flags |= settings.borderless ? SDL_WINDOW_BORDERLESS : 0;
        flags |= settings.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
        flags |= settings.resizable ? SDL_WINDOW_RESIZABLE : 0;
        flags |= settings.overrideFlags;
        window = SDL_CreateWindow(settings.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  settings.width, settings.height, flags);

        if (window == nullptr)
        {
            NGIN_ERROR("Failed to create window: {}", SDL_GetError());
            return false;
        }

        isInitialized = true;
        return true;
    }

    void Window::Shutdown()
    {
        if (!isInitialized)
        {
            NGIN_WARNING("Cannot shutdown window that is not initialized. Stopping shutdown.");
            return;
        }

        if (window)
            SDL_DestroyWindow(window);
        window = nullptr;
        isInitialized = false;
    }


    bool Window::IsOpen() const
    {
        return isInitialized;
    }

    int Window::GetWidth() const
    {
        if (window) [[likely]]
        {
            int width;
            SDL_GetWindowSize(window, &width, nullptr);
            return width;
        }
        return 0;
    }

    int Window::GetHeight() const
    {
        if (window) [[likely]]
        {
            int height;
            SDL_GetWindowSize(window, nullptr, &height);
            return height;
        }
        return 0;
    }

    SDL_Window* Window::GetSDLWindow() const
    {
        return window;
    }

    Void Window::Resize(UInt32 width, UInt32 height)
    {
        SDL_SetWindowSize(window, static_cast<Int32>(width), static_cast<Int32>(height));

    }
} // namespace NGIN
