#pragma once
#include <iostream>
#include <string_view>
#include <cstddef>

// https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way  (Answer by vrqq)
namespace NGIN::Meta
{
    template <typename T>
    struct TypeName
    {

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

        constexpr static std::string_view Name()
        {
            size_t prefix_len = TypeName<void>::Signature().find("void");
            size_t multiple = TypeName<void>::Signature().size() - TypeName<int>::Signature().size();
            size_t dummy_len = TypeName<void>::Signature().size() - 4 * multiple;
            size_t target_len = (Signature().size() - dummy_len) / multiple;
            std::string_view rv = Signature().substr(prefix_len, target_len);
            if (rv.rfind(' ') == rv.npos)
                return rv;
            return rv.substr(rv.rfind(' ') + 1);
        }

        using type = T;
        constexpr static std::string_view value = Name();
    };

} // namespace NGIN::Meta