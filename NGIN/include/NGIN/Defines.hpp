#pragma once
#include <source_location>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <source_location>


/**
 * @brief NGIN_API is a macro used for defining the visibility of functions and classes in the NGIN library.
 *
 * When building the NGIN library, this macro is defined as "__declspec(dllexport)" to export symbols from the library.
 * When using the NGIN library, this macro is defined as "__declspec(dllimport)" to import symbols from the library.
 */
#if defined(_MSC_VER)
#ifdef NGIN_EXPORTS
#define NGIN_API __declspec(dllexport)
#else
#define NGIN_API __declspec(dllimport)
#endif
#else
#ifdef NGIN_EXPORTS
#define NGIN_API __attribute__((visibility("default")))
#else
#define NGIN_API
#endif
#endif
namespace NGIN
{
    using UInt64 = std::uint64_t;
    using UInt32 = std::uint32_t;
    using UInt16 = std::uint16_t;
    using UInt8 = std::uint8_t;

    using Int64 = std::int64_t;
    using Int32 = std::int32_t;
    using Int16 = std::int16_t;
    using Int8 = std::int8_t;

    using Byte = std::byte;

    using Void = void;


    using F32 = float;
    using F64 = double;

    using String = std::string;
    using StringView = std::string_view;

    using Bool = bool;

    using SourceLocation = std::source_location;
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using WeakRef = std::weak_ptr<T>;
}
