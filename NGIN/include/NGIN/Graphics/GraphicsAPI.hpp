#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Graphics
{
    /// @brief Enumerates the supported graphics APIs.
    enum class GraphicsAPI : UInt8
    {
        NONE = 0,
        VULKAN,
        OPENGL,
        DX12
    };
} // namespace NGIN::Graphics
