#include <Precompiled/PCH.h>
#include <NGIN/Graphics/Context.hpp>
#include <NGIN/Graphics/Renderer.hpp>
#include <NGIN/Graphics/Vulkan/Renderer.hpp>
#include <NGIN/Graphics/Window.hpp>
#include <NGIN/Logging.hpp>

namespace NGIN::Graphics
{


    Context::~Context()
    {
        if (renderer)
            delete renderer;
    }

    bool Context::Init(GraphicsAPI backend, Window* window)
    {
        this->window = window;


        switch (backend)
        {
            case GraphicsAPI::OPEN_GL:
            case GraphicsAPI::D3D12:
            case GraphicsAPI::VULKAN:
                renderer = new Vulkan::Renderer(*window);
                break;


            default:
                break;
        }
        return renderer->Init();
    }

    void Context::Shutdown()
    {
        if (renderer)
            delete renderer;
        renderer = nullptr;
        window = nullptr;
    }

}