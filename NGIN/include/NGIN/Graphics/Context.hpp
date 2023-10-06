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

        Context() = default;

        virtual ~Context() = default;


        /// @brief Initialize the context.
        /// @return True if the context was initialized successfully, false otherwise.
        virtual Bool Init() = 0;

        /// @brief Shutdown the context.
        /// @return True if the context was shutdown successfully, false otherwise.
        virtual Bool Shutdown() = 0;

        /// @brief Set up a swapchain for the given window.
        /// @param window The window for which to set up a swapchain.
        virtual Bool SetupSwapchain(Ref<Window> window) = 0;

        /// @brief Get the renderer
        /// @return The renderer
        [[nodiscard]] virtual Renderer& GetRenderer() const = 0;

    };
}
