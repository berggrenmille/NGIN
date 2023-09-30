#pragma once

#include <string>
#include <vector>

namespace NGIN::Meta::Reflection::Types
{
    /// \brief Represents metadata for an enumeration within the reflection system.
    struct Enum
    {
        /// \name of the enumeration.
        std::string name;

        /// \brief List of name-value pairs representing the enumeration items.
        /// Each item consists of the item's name and its corresponding integer value.
        std::vector<std::pair<std::string, int>> enumerators;
    };
}