#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/GraphicsAPI.hpp>
#include <NGIN/Graphics/Surface.hpp>

#include <string>
#include <vector>

struct SDL_Window;

namespace NGIN::Graphics
{
    struct WindowConfig
    {
        String title = "NGIN";
        Int32 width = 1280;
        Int32 height = 720;
        GraphicsAPI api = GraphicsAPI::VULKAN;
        Bool fullscreen = false;
        Bool resizable = false;
        Bool borderless = false;
    };

    class Window : public Surface
    {
    public:
        Window() = default;

        ~Window() override;

        Bool Init(WindowConfig& config);

        Void Shutdown();

        Void Resize(UInt32 width, UInt32 height);

        [[nodiscard]]  SDL_Window* GetSDLWindow() const;

        void GetDimensions(int& width, int& height) const override;

        [[nodiscard]]   Bool IsOpen() const;

    private:
        SDL_Window* sdlWindow = nullptr;

    };
}