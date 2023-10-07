#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Context.hpp>
#include <NGIN/Graphics/Renderer.hpp>

#include <NGIN/Graphics/Platform/SDL/Window.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{


    bool Context::Init(GraphicsAPI backend, WindowConfig* windowSettings)
    {
        if (!windowSettings)
        {
            NGIN_ERROR("Window settings is nullptr");
            return false;
        };

        switch (backend)
        {
            case GraphicsAPI::OPENGL:
                windowSettings->api = GraphicsAPI::OPENGL;
                break;
            case GraphicsAPI::VULKAN:
                windowSettings->api = GraphicsAPI::VULKAN;
                break;
            case GraphicsAPI::DX12:
                windowSettings->api = GraphicsAPI::DX12;
                break;
            default:
                NGIN_ERROR("Invalid graphics API");
                return false;
        }

        window = Ref<Window>(new Window());

        if (!window->Init(*windowSettings))
        {
            NGIN_ERROR("Failed to initialize window");
            return false;
        }

        switch (backend)
        {
            case GraphicsAPI::OPENGL:
            case GraphicsAPI::DX12:
            case GraphicsAPI::VULKAN:
                //renderer = CreateRef<Vulkan::VulkanRenderer>(window);
                break;


            default:
                break;
        }
        return true;
        // return renderer->Init();
    }

    void Context::Shutdown()
    {
        //Release all resources
        renderer.reset();
        window.reset();
    }

}