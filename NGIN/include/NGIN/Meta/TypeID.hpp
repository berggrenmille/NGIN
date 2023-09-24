#pragma once
#include <NGIN/Defines.hpp>
#include <cstddef>

namespace NGIN::Meta
{
    using TypeIDType = UInt64;

    template <typename T>
    struct TypeIDResolver
    {
        static void ID()
        {
            volatile int dummy = 0;

        }
    };

    /// @brief Fetches a unique type ID for a given type at runtime.
    ///
    /// This function provides a mechanism to generate a pseudo-unique type ID
    /// based on the memory address of a static variable inside `TypeIDResolver`. This ensures
    /// that the ID remains consistent during a single execution of the program.
    ///
    /// @tparam T The type for which the ID should be fetched.
    /// @return The unique type ID for the specified type.
    template <typename T>
    [[nodiscard]] TypeIDType TypeID() noexcept
    {
        static volatile auto id = TypeIDResolver<T>::ID;
        return reinterpret_cast<TypeIDType>(&id);
    }
}