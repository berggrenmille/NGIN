#pragma once

#include <NGIN/Defines.hpp>
#include "GraphicsAPI.hpp"
#include <NGIN/Graphics/Window.hpp>

namespace NGIN::Graphics
{
    class Renderer;


    class NGIN_API Context
    {
    public:

        bool Init(GraphicsAPI backend, WindowConfig* windowSettings);

        void Shutdown();

        void Tick();


        [[nodiscard]] inline Ref<Window> GetWindow() const { return window; }

        [[nodiscard]] inline Ref<Renderer> GetRenderer() const { return renderer; }

    private:
        Ref<Renderer> renderer {nullptr};
        Ref<Window> window {nullptr};
    };
}
