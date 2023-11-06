#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Graphics
{
    class IRenderer;

    enum class eGraphicsAPI : UInt8
    {
        NONE = 0,
        VULKAN,
        OPENGL,
        DX12
    };

    class NGIN_API IContext
    {
    public:
        virtual ~IContext() = default;


        /// @brief Initialize the context.
        /// @return True if the context was initialized successfully, false otherwise.
        virtual Bool Init() = 0;

        /// @brief Shutdown the context.
        virtual void Shutdown() = 0;

        /// @brief Begin a new frame.
        virtual void BeginFrame() = 0;

        /// @brief End the current frame.
        virtual void EndFrame() = 0;

        /// @brief Present the current frame.
        /// This is only applicable for contexts that are double/triple buffered.
        virtual void Present() = 0;

        /// @brief Set the viewport.
        /// @param[in] x The x coordinate of the viewport.
        /// @param[in] y The y coordinate of the viewport.
        /// @param[in] width The width of the viewport.
        /// @param[in] height The height of the viewport.
        virtual void SetViewport(UInt32 x, UInt32 y, UInt32 width, UInt32 height) = 0;

        /// @brief Set the clear color.
        /// @param[in] r The red component of the clear color.
        /// @param[in] g The green component of the clear color.
        /// @param[in] b The blue component of the clear color.
        /// @param[in] a The alpha component of the clear color.
        virtual void SetClearColor(float r, float g, float b, float a) = 0;

        /// @brief Clear the current surface.
        virtual void Clear() = 0;


        /// @brief Get the renderer
        /// @return The renderer
        [[nodiscard]] virtual IRenderer& GetRenderer() const = 0;

        /// @brief Get the name of the current graphics API in use.
        /// @return A String containing the current GraphicsApi in use.
        [[nodiscard]] virtual String GetAPIName() const = 0;

        /// @brief Get the current graphics API in use.
        /// @return The current GraphicsApi in use.
        [[nodiscard]] virtual eGraphicsAPI GetAPI() const = 0;


    protected:
        IContext() = default;
    };
}// namespace NGIN::Graphics
