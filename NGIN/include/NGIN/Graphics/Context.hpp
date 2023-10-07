#pragma once

#include <NGIN/Defines.hpp>
#include "GraphicsAPI.hpp"
#include <NGIN/Graphics/Window.hpp>

namespace NGIN::Graphics
{
    class Renderer;vulkanpipeline

    RendererAP
    class NGIN_API Context
    {
    public:

        Context() = default;

        virtual ~Context() = default;


        /// @brief Initialize the context.
        /// @return True if the context was initialized successfully, false otherwise.
        virtual Bool Init() = 0;

        /// @brief Shutdown the context.
        /// @return True if the context was shutdown successfully, false otherwise.
        virtual Bool Shutdown() = 0;

        /// @brief Get the renderer
        /// @return The renderer
        [[nodiscard]] virtual Renderer& GetRenderer() const = 0;

        [[nodiscard]] virtual String GetAPIName() const = 0;

    };
}
