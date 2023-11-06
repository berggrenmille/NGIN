//
// Created by Maximiliam Berggren on 06/11/2023.
//

#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Graphics
{
    enum class eDeviceFeatures : UInt32
    {
        MULTI_DRAW_INDIRECT       = 1 << 0,     // Ability to issue multiple draw calls in a single command.
        GEOMETRY_SHADER           = 1 << 1,     // Support for geometry shaders.
        TESSELLATION_SHADER       = 1 << 2,     // Support for tessellation shaders.
        COMPUTE_SHADER            = 1 << 3,     // Support for compute shaders.
        MULTI_VIEWPORT            = 1 << 4,     // Support for rendering to multiple viewports.
        ANISOTROPIC_FILTERING     = 1 << 5,     // Support for anisotropic texture filtering.
        SAMPLER_MIPMAP_LOD_BIAS   = 1 << 6,     // Support for mipmap level of detail biasing.
        SHADER_FLOAT64            = 1 << 7,     // Support for double precision shaders.
        SHADER_INT64              = 1 << 8,     // Support for 64-bit integer shaders.
        SHADER_INT16              = 1 << 9,     // Support for 16-bit integer shaders.
        TEXTURE_COMPRESSION_BC    = 1 << 10,    // Support for block-compression texture formats.
        OCCLUSION_QUERY_PRECISE   = 1 << 11,    // Support for precise occlusion queries.
        PIPELINE_STATISTICS_QUERY = 1 << 12,    // Support for pipeline statistics queries.
        VERTEX_ATTRIBUTE_DIVISOR  = 1 << 13,    // Support for instance rate divisor in vertex attributes.
        RENDER_TO_TEXTURE         = 1 << 14,    // Support for rendering to textures directly.
        TEXTURE_ARRAYS            = 1 << 15,    // Support for texture arrays.
        CUBE_MAP_ARRAYS           = 1 << 16,    // Support for cubemap arrays.
        INDEPENDENT_BLENDING      = 1 << 17     // Support for independent blending modes for multiple render targets.
    };

    enum class eDeviceType : UInt8
    {
        NONE = 0,
        INTEGRATED,
        DISCRETE,
        VIRTUAL,
        SOFTWARE
    };

    class NGIN_API IDevice
    {
    public:
        virtual ~IDevice() = default;

        [[nodiscard]] virtual const String& GetDeviceName() const = 0;

        [[nodiscard]] virtual const String& GetVendorName() const = 0;

        [[nodiscard]] virtual const String& GetDriverVersion() const = 0;

        [[nodiscard]] virtual const String& GetDriverDate() const = 0;

        [[nodiscard]] virtual eDeviceType GetType() const = 0;

        [[nodiscard]] virtual UInt32 GetFeatures() const = 0;
    };
}

