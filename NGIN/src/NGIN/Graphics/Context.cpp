#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Context.hpp>
#include <NGIN/Graphics/Renderer.hpp>
#include <NGIN/Graphics/Vulkan/VulkanRenderer.hpp>
#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{


    Context::Context(GraphicsBackend backend, const std::string& title, int width, int height)
    {
        window = new Window();
        if (!window->Init(backend, title, width, height))
        {
            NGIN_ERROR("Failed to initialize window!");
            return;
        }



        switch (backend)
        {
        case GraphicsBackend::OPEN_GL:
        case GraphicsBackend::D3D12:
        case GraphicsBackend::VULKAN:
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