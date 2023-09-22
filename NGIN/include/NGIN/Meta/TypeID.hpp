#include <NGIN/Defines.hpp>
#include <cstddef>

namespace NGIN::Meta
{
    using TypeIDType = Size;

    template <typename T>
    struct TypeIDResolver
    {
        static void ID() {}
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