#pragma once

#include <string>
#include <cstddef>

namespace NGIN::Meta::Reflection::Types
{
    /// @brief Contains metadata about a field in a class
    struct Field
    {
        std::string name;
        std::string type;
        size_t offset;

    };
}