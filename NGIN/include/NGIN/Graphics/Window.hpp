#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/GraphicsAPI.hpp>
#include <NGIN/Graphics/Surface.hpp>
#include <NGIN/Meta/UUID.hpp>

#include <string>
#include <vector>

struct SDL_Window;

namespace NGIN::Graphics
{

    /// \struct WindowConfig
    /// \brief Configuration properties for a window.
    /// \note This struct is subject to change.
    struct WindowConfig
    {
        String      title      = "NGIN";
        UInt32      width      = 1280;
        UInt32      height     = 720;
        GraphicsAPI api        = GraphicsAPI::NONE;
        Bool        fullscreen = false;
        Bool        resizable  = false;
        Bool        borderless = false;
    };


    /// \class Window
    /// \brief Abstraction for a window.
    ///
    /// This class is used to create and manage a window.
    /// Internally it uses SDL for window creation and management
    /// \note This class exposes the SDL_Window* handle, which might change in the future.
    class Window : public Surface
    {
    public:
        Window() = default;

        ~Window() override;

        Bool Init(const WindowConfig& config);

        Void Shutdown();

        Void Resize(UInt32 width, UInt32 height);

        [[nodiscard]] void* GetNativeHandle() const override;

        [[nodiscard]] SDL_Window* GetSDLWindow() const;

        void GetDimensions(UInt32& outWidth, UInt32& outHeight) const override;

        [[nodiscard]] Bool IsOpen() const;

    private:
        SDL_Window* sdlWindow = nullptr;
        Meta::UUID uuid = {};
    };
}