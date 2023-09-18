#pragma once
// NGIN include(s)
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
        std::string name;
        size_t size;
        size_t alignment;
        size_t version;
        void *(*ctor)(void *);
        void (*dtor)(void *);
        std::vector<Field> fields;
        std::vector<Function> functions;
    };
}