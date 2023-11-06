#pragma once

#include <source_location>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <source_location>
#include <vector>
#include <span>
#include <deque>
#include <list>
#include <queue>
#include <unordered_map>
#include <map>


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
    // TODO: Add documentation


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

    using Size = std::size_t;
    using IntPtr = std::intptr_t;
    using UIntPtr = std::uintptr_t;

    using String = std::string;
    using StringView = std::string_view;

    using Char = char;
    using WChar = wchar_t;

    using Int = int;
    using UInt = unsigned int;

    using Long = long;
    using ULong = unsigned long;


    using Bool = bool;

    using SourceLocation = std::source_location;

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Ticket = std::weak_ptr<T>;

    /// @brief Constructs an object and creates a Scope.
    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /// @brief Constructs an object and creates a Ref.
    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /// @brief Creates a Ticket from a Ref.
    template<typename T>
    constexpr Ticket<T> CreateTicket(const Ref<T>& ptr)
    {
        return TicketPtr<T>(ptr);
    }

    template<typename T>
    using Vector = std::vector<T>;
    template<typename T>
    using Span = std::span<T>;
    template<typename T>
    using Deque = std::deque<T>;
    template<typename T>
    using List = std::list<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename Key, typename Value>
    using UnorderedMap = std::unordered_map<Key, Value>;
    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;

}
