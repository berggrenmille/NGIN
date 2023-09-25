#pragma once
#include <NGIN/Defines.hpp>
#include <cstddef>

namespace NGIN::Meta
{
    using TypeIDType = UInt64;

    /// @brief Resolver for generating unique Type ID.
    ///
    /// This class is used to hold the ID() function that is used
    /// to generate a unique Type ID at runtime. The ID function uses compiler-specific
    /// mechanisms to ensure it is not optimized away.
    template <typename T>
    struct TypeIDResolver
    {
        /// @brief Dummy function to ensure unique memory address for each type.
        ///
        /// The function does nothing but is designed in a way to ensure
        /// it won't be optimized away, serving as a basis for unique type IDs.
        static void ID()

        {
#if defined(__GNUC__) || defined(__clang__)
            asm volatile("");
#elif defined(_MSC_VER)
            __asm nop;
#endif
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
        return reinterpret_cast<TypeIDType>(&TypeIDResolver<T>::ID);
    }
}