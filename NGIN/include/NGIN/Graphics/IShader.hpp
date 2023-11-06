// IShader.hpp

#pragma once

#include <NGIN/Defines.hpp>
#include <string>

namespace NGIN::Graphics
{

    enum class eShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER,
        TESSELLATION_CONTROL_SHADER,
        TESSELLATION_EVALUATION_SHADER,
        COMPUTE_SHADER
    };

    class NGIN_API IShader
    {
    public:
        virtual ~IShader() = default;

        /// @brief Load shader source code from a file.
        /// @param filename Path to the shader source code file.
        virtual Bool LoadFromFile(const String& filename, eShaderType type) = 0;

        /// @brief Load shader source code from a string.
        /// @param source Shader source code string.
        virtual Bool LoadFromSource(const String& source, eShaderType type) = 0;

        /// @brief Compile the shader and check for errors.
        virtual Bool Compile() = 0;

        /// @brief Get the compiled shader's ID.
        /// @return Shader ID used by the graphics API.
        [[nodiscard]] virtual UInt32 GetShaderID() const = 0;

        /// @brief Check if the shader is currently compiled and valid.
        [[nodiscard]] virtual Bool IsValid() const = 0;

        /// @brief Get the last compilation log (errors, warnings, etc.).
        [[nodiscard]] virtual String GetCompilationLog() const = 0;
    };
}
