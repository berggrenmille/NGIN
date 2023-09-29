#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/GraphicsBackend.hpp>

#include <SDL2/SDL.h>
#include <string>

namespace NGIN::Graphics
{

    struct WindowSettings
    {
        String title = "NGIN";
        Int32 width = 1280;
        Int32 height = 720;
        Bool fullscreen = false;
        Bool resizable = false;
        Bool borderless = false;
    };

    class Window
    {
    public:
        NGIN_API Window() = default;

        NGIN_API ~Window() = default;

        NGIN_API Bool Init(GraphicsAPI backend, WindowSettings& settings);

        NGIN_API Void Shutdown();

        NGIN_API Void Resize(UInt32 width, UInt32 height);

        [[nodiscard]] NGIN_API Bool IsOpen() const;

        [[nodiscard]] NGIN_API Int32 GetWidth() const;

        [[nodiscard]] NGIN_API Int32 GetHeight() const;

        [[nodiscard]] NGIN_API SDL_Window* GetSDLWindow() const;

    private:
        SDL_Window* window = nullptr;
        bool isInitialized = false;
    };
}