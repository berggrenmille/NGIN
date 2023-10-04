#pragma once
// NGIN include(s)
#include <NGIN/Defines.hpp>
#include "Field.hpp"
#include "Function.hpp"
// STL include(s)
#include <string>
#include <vector>
#include <cstddef>

namespace NGIN::Meta::Reflection::Types
{

    struct Class
    {
        String name;
        UInt64 size;
        UInt64 alignment;
        UInt64 version;

        void* (* ctor)(void*);

        void (* dtor)(void*);

        std::vector<Field> fields;
        std::vector<Function> functions;
    };
}