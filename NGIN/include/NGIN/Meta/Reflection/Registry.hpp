#pragma once
#include <NGIN/Core.h>
#include "Types.hpp"

#include <unordered_map>

namespace NGIN::Meta::Reflection
{
    class Registry
    {
    public:
        NGIN_API static Registry &GetInstance();

        NGIN_API void AddClass(const Types::Class &classData);

        NGIN_API const Types::Class &GetClassFromString(std::string className);

    private:
        std::unordered_map<std::string, Types::Class> classMap;
    };
}
