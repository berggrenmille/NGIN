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
    using uint64 = std::uint64_t;
    using uint32 = std::uint32_t;
    using uint16 = std::uint16_t;
    using uint8 = std::uint8_t;

    using int64 = std::int64_t;
    using int32 = std::int32_t;
    using int16 = std::int16_t;
    using int8 = std::int8_t;

    using byte = std::byte;

    using float32 = float;
    using float64 = double;

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using WeakRef = std::weak_ptr<T>;
}
