#pragma once
#include "Internal/AllocatorConcepts.hpp"
namespace NGIN::Memory
{
    template <Internal::IsAllocator PrimaryT, Internal::IsAllocator FallbackT>
    class FallbackAllocator
    {
    public:
        FallbackAllocator(PrimaryT &&primary, FallbackT &&fallback)
            : primary(std::move(primary)), fallback(std::move(fallback)) {}

        FallbackAllocator(FallbackAllocator &&other) noexcept
            : primary(std::move(other.primary)), fallback(std::move(other.fallback)) {}

        FallbackAllocator &operator=(FallbackAllocator &&other) noexcept
        {
            primary = std::move(other.primary);
            fallback = std::move(other.fallback);
            return *this;
        }

        void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t),
                       const std::source_location &location = std::source_location::current())
        {
            void *ptr = primary.Allocate(size, alignment, location);
            if (!ptr)
                ptr = fallback.Allocate(size, alignment, location);
            return ptr;
        }

        void Deallocate(void *ptr)
        {

            if (primary.Owns(ptr))
            {
                if constexpr (Internal::HasDeallocate<PrimaryT>)
                    primary.Deallocate(ptr);
            }
            else if (fallback.Owns(ptr))
            {
                if constexpr (Internal::HasDeallocate<FallbackT>)
                    fallback.Deallocate(ptr);
            }
        }

        void DeallocateAll()
        {
            if constexpr (Internal::HasDeallocateAll<PrimaryT>)
                primary.DeallocateAll();
            if constexpr (Internal::HasDeallocateAll<FallbackT>)
                fallback.DeallocateAll();
        }

        bool Owns(void *ptr) const
        {
            return primary.Owns(ptr) || fallback.Owns(ptr);
        }

    private:
        PrimaryT primary;
        FallbackT fallback;
    };
}