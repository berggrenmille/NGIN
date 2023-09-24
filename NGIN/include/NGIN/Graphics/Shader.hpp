#pragma once
#include <NGIN/Defines.hpp>
namespace NGIN::Graphics
{
    enum class ShaderType : UInt8
    {
        VERTEX,
        FRAGMENT,
        COMPUTE
        //... other shader types
    };

    class Shader
    {
    public:
        Shader(const String& path, ShaderType type);
        ~Shader();

        const String& GetPath() const { return path; }
        ShaderType GetType() const { return type; }
    private:
        String path;
        ShaderType type;
    }
}