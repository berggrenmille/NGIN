#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Context.hpp>
#include <NGIN/Graphics/Renderer.hpp>
#include <NGIN/Graphics/Vulkan/Renderer.hpp>
#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{


    bool Context::Init(GraphicsAPI backend, WindowSettings* windowSettings)
    {
        if (!windowSettings)
        {
            NGIN_ERROR("Window settings is nullptr");
            return false;
        };

        switch (backend)
        {
            case GraphicsAPI::OPEN_GL:
                windowSettings->overrideFlags |= SDL_WINDOW_OPENGL;
                break;
            case GraphicsAPI::VULKAN:
                windowSettings->overrideFlags |= SDL_WINDOW_VULKAN;
                break;
            case GraphicsAPI::D3D12:
            default:
                NGIN_ERROR("Invalid graphics API");
                return false;
        }

        window = CreateRef<Window>();

        if (!window->Init(*windowSettings))
        {
            NGIN_ERROR("Failed to initialize window");
            return false;
        }

        switch (backend)
        {
            case GraphicsAPI::OPEN_GL:
            case GraphicsAPI::D3D12:
            case GraphicsAPI::VULKAN:
                renderer = CreateRef<Vulkan::Renderer>(window);
                break;


            default:
                break;
        }
        return renderer->Init();
    }

    void Context::Shutdown()
    {
        //Release all resources
        renderer.reset();
        window.reset();
    }

}