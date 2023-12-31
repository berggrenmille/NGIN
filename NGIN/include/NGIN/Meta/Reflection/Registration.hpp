#pragma once

#include "ClassRegistrar.hpp"
#include "EnumRegistrar.hpp"

#if defined(__clang__)
#define NGIN_DECLARE_SHARED_CTOR __attribute__((__constructor__))
#define NGIN_DECLARE_SHARED_DTOR __attribute__((__destructor__))
#elif defined(_MSC_VER)
#define NGIN_DECLARE_SHARED_CTOR
#define NGIN_DECLARE_SHARED_DTOR
#else
#define NGIN_DECLARE_SHARED_CTOR __attribute__((constructor))
#define NGIN_DECLARE_SHARED_DTOR __attribute__((destructor))
#endif

#define NGIN_REFLECTION_REGISTRATION()              \
    static void NGIN_AUTO_REGISTRATION_FUNCTION();  \
    namespace                                       \
    {                                               \
        struct NGIN_AUTO_REGISTRATION_STRUCT        \
        {                                           \
            NGIN_AUTO_REGISTRATION_STRUCT()         \
            {                                       \
                NGIN_AUTO_REGISTRATION_FUNCTION();  \
            }                                       \
        };                                          \
        static NGIN_AUTO_REGISTRATION_STRUCT        \
            NGIN_AUTO_REGISTRATION_STRUCT_INSTANCE; \
    }                                               \
    static void NGIN_AUTO_REGISTRATION_FUNCTION()

namespace NGIN::Meta::Reflection
{
    template<typename T>
    static ClassRegistrar<T> RegisterClass()
    {
        return ClassRegistrar<T>();
    }

    template<typename T>
    static EnumRegistrar<T> RegisterEnum()
    {
        return EnumRegistrar<T>();
    }
} // namespace NGIN::Meta::Reflection