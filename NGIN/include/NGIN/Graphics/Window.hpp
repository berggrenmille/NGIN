#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/GraphicsAPI.hpp>
#include <NGIN/Graphics/ISurface.hpp>
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
        String      name       = "Primary";
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
        NGIN_API Window() = default;

        NGIN_API ~Window() override;

        NGIN_API Bool Init(const WindowConfig& config);

        NGIN_API Void Shutdown();

        NGIN_API Void Resize(UInt32 width, UInt32 height);

        [[nodiscard]] NGIN_API void* GetNativeHandle() const override;

        [[nodiscard]] NGIN_API SDL_Window* GetSDLWindow() const;

        NGIN_API void GetDimensions(UInt32& outWidth, UInt32& outHeight) const override;

        [[nodiscard]] NGIN_API Bool IsOpen() const;

        [[nodiscard]] NGIN_API Meta::UUID GetUUID() const;

        [[nodiscard]] NGIN_API const String& GetName() const;

    private:
        SDL_Window* sdlWindow = nullptr;
        Meta::UUID uuid = {};
        String     name;
    };
}