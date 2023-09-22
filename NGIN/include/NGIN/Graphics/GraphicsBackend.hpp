#pragma once
#include <NGIN/Defines.hpp>

namespace NGIN::Graphics
{
    enum class GraphicsBackend : UInt8
    {
        VULKAN,
        OPEN_GL,
        D3D12,
        //... other backends
    };
} // namespace NGIN::Graphics
