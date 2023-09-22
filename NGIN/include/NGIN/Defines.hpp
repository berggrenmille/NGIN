#pragma once
#include <source_location>
#include <cstddef>
#include <cstdint>
#include <memory>

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

    using F32 = float;
    using F64 = double;

    using Size = std::size_t;

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using WeakRef = std::weak_ptr<T>;
}
