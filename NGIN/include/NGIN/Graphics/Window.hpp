#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/GraphicsAPI.hpp>
#include <NGIN/Graphics/Surface.hpp>

#include <string>
#include <vector>

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
        NGIN_API Window() = default;

        NGIN_API virtual  ~Window() = default;

        NGIN_API virtual Bool Init(WindowConfig& config) = 0;

        NGIN_API virtual Void Shutdown() = 0;

        NGIN_API virtual Void Resize(UInt32 width, UInt32 height) = 0;

        [[nodiscard]] NGIN_API virtual Bool IsOpen() const;

        [[nodiscard]] NGIN_API virtual void* GetNativeHandle() const = 0;

        NGIN_API virtual void GetDimensions(int& width, int& height) const = 0;


    };
}