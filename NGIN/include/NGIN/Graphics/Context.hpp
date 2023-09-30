#pragma once

#include <NGIN/Defines.hpp>
#include "GraphicsBackend.hpp"
#include <NGIN/Graphics/Window.hpp>

namespace NGIN::Graphics
{
    class Renderer;


    class NGIN_API Context
    {
    public:
        Context() = default;


        ~Context();

        bool Init(GraphicsAPI backend, Window* windowSettings);

        void Shutdown();

        void Tick();


        [[nodiscard]] inline Window* GetWindow() const { return window; }

        [[nodiscard]] inline Renderer* GetRenderer() const { return renderer; }

    private:
        Renderer* renderer = nullptr;
        Window* window = nullptr;
    };
}
