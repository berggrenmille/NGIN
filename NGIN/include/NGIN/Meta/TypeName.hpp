#pragma once
#include <string_view>
#include <type_traits>

namespace NGIN::Meta
{

    /// @brief Retrieves the type name as a string.
    ///
    /// This structure provides functionality to get the type name
    /// of a given type as a compile-time `std::string_view`.
    /// @tparam T The type whose name is to be retrieved.
    ///
    /// @example
    /// ```cpp
    /// std::cout << TypeName<int>::value; // Outputs: int
    /// ```
    template <typename T>
    struct TypeName
    {
        /// @brief Gets the compiler-specific signature for the current type.
        ///
        /// Depending on the compiler being used, this function will retrieve
        /// the appropriate signature string.
        ///
        /// @return A `std::string_view` containing the compiler-specific signature for the type.
        constexpr static std::string_view Signature()
        {
#if defined(__clang__) || defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
        }

        /// @brief Computes and returns the name of the type.
        ///
        /// The function computes the name of the type based on the compiler
        /// signature and performs string manipulation to retrieve a clean
        /// version of the type name.
        ///
        /// @return A `std::string_view` containing the name of the type.
        constexpr static std::string_view Full()
        {
            if constexpr (std::is_same_v<T, void *>)
                return "void*";
            if constexpr (std::is_same_v<T, void>)
                return "void";

            size_t prefix_len = TypeName<void>::Signature().find("void");
            size_t multiple = TypeName<void>::Signature().size() - TypeName<int>::Signature().size();
            size_t dummy_len = TypeName<void>::Signature().size() - 4 * multiple;
            size_t target_len = (Signature().size() - dummy_len) / multiple;
            std::string_view rv = Signature().substr(prefix_len, target_len);
            if (rv.rfind(' ') == rv.npos)
                return rv;
            return rv.substr(rv.rfind(' ') + 1);
        }

        /// @brief Computes and returns the class name of the type.
        ///
        /// The function computes the class name of the type based on the type signature
        /// and performs string manipulation to retrieve a clean version of the class name.
        ///
        /// @return A `std::string_view` containing the class name.
        constexpr static std::string_view Class()
        {
            std::string_view fullType = Full();
            size_t lastColons = fullType.rfind("::");
            if (lastColons == std::string_view::npos)
                return fullType;
            return fullType.substr(lastColons + 2);
        }

        /// @brief Computes and returns the namespace of the type.
        ///
        /// The function computes the namespace of the type based on the type signature
        /// and performs string manipulation to retrieve the namespace.
        ///
        /// @return A `std::string_view` containing the namespace.
        constexpr static std::string_view Namespace()
        {
            std::string_view fullType = Full();
            size_t lastColons = fullType.rfind("::");
            if (lastColons == std::string_view::npos)
                return {};
            return fullType.substr(0, lastColons);
        }
    };

} // namespace NGIN::Meta
