#pragma once

#include <NGIN/Defines.hpp>
#include "Types.hpp"

#include <unordered_map>

namespace NGIN::Meta::Reflection
{
    class Registry
    {
    public:
        NGIN_API static Registry& GetInstance();

        NGIN_API void AddClass(const Types::Class& classData);

        NGIN_API void AddEnum(const Types::Enum& enumData);


        NGIN_API const Types::Class& GetClassFromString(const String& className);

        NGIN_API const Types::Enum& GetEnumFromString(const String& enumName);

    private:
        std::unordered_map<String, Types::Class> classMap;
        std::unordered_map<String, Types::Enum> enumMap;
    };
}
