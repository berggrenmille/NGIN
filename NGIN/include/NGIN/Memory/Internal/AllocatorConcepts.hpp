#pragma once
#include <concepts>
#include <source_location>

namespace NGIN::Memory
{
    /**
     * @namespace Internal
     * @brief Contains internal concepts to check for expected behaviors.
     */
    namespace Internal
    {

        template <typename T>
        concept HasAllocate = requires(T a, size_t size, size_t alignment, const std::source_location &location) {
            {
                a.Allocate(size, alignment, location)
            } -> std::same_as<void *>;
        };

        template <typename T>
        concept HasDeallocate = requires(T a, void *ptr) {
            {
                a.Deallocate(ptr)
            } -> std::same_as<void>;
        };

        template <typename T>
        concept HasDeallocateAll = requires(T a) {
            {
                a.DeallocateAll()
            } -> std::same_as<void>;
        };

        template <typename T>
        concept HasOwns = requires(T a, void *ptr) {
            {
                a.Owns(ptr)
            } -> std::same_as<bool>;
        };

        template <typename T>
        concept IsAllocator = HasAllocate<T> && HasOwns<T>;
    } // namespace Internal
}
