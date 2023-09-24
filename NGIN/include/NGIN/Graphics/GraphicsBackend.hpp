#pragma once
#include <NGIN/Defines.hpp>

namespace NGIN::Graphics
{
    enum class GraphicsAPI : UInt8
    {
        NONE = 0,
        VULKAN,
        OPEN_GL,
        D3D12
        //... other backends
    };
} // namespace NGIN::Graphics
