#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Graphics
{
    class IRenderer;

    class NGIN_API IContext
    {
    public:
        virtual ~IContext() = default;


        /// @brief Initialize the context.
        /// @return True if the context was initialized successfully, false otherwise.
        virtual Bool Init() = 0;

        /// @brief Shutdown the context.
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear() = 0; 


        /// @brief Get the renderer
        /// @return The renderer
        [[nodiscard]] virtual IRenderer& GetRenderer() const = 0;

        /// @brief Get the name of the current graphics API in use.
        /// @return A String containing the current GraphicsApi in use.
        [[nodiscard]] virtual String GetAPIName() const = 0;
    protected:
        IContext() = default;
    };
}// namespace NGIN::Graphics
