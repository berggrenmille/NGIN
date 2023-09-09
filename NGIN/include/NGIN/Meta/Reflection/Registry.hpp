#pragma once
#include "Types.hpp"
#include <unordered_map>

namespace NGIN::Meta::Reflection
{
    class Registry
    {
    public:
        static Registry &GetInstance();

        void AddClass(const Class &classData);

    private:
        std::unordered_map<std::string, Class> classMap;
    };
}
