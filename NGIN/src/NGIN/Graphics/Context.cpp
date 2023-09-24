#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Context.hpp>
#include <NGIN/Graphics/Renderer.hpp>
#include <NGIN/Graphics/Vulkan/VulkanRenderer.hpp>
#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{


    Context::Context(GraphicsAPI backend, WindowSettings* windowSettings, const SourceLocation& srcLocation)
    {
        if (!windowSettings)
        {
            NGIN_LOG_SRC(srcLocation, NGIN::Logging::Level::Warning, "Window settings are null. Cannot initialize window.");
            return;
        }
        window = new Window();

        if (!window->Init(backend, *windowSettings))
        {
            NGIN_ERROR("Failed to initialize window!");
            return;
        }



        switch (backend)
        {
        case GraphicsAPI::OPEN_GL:
        case GraphicsAPI::D3D12:
        case GraphicsAPI::VULKAN:
            renderer = new VulkanRenderer(*window);
            break;


        default:
            break;
        }
    }

    Context::~Context()
    {
        if (renderer)
            delete renderer;
    }

}