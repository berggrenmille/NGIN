#pragma once
#include <NGIN/Defines.hpp>

namespace NGIN::Memory
{
    class NGIN_API Mallocator
    {
    public:
        // Mallocator does not need any initialization, so the constructor is empty
        Mallocator() = default;
        Mallocator(Mallocator &&other) noexcept = default;
        Mallocator &operator=(Mallocator &&other) noexcept;

        void *Allocate(size_t size,
                       size_t alignment = alignof(std::max_align_t),
                       const std::source_location &location = std::source_location::current());

        void Deallocate(void *ptr);

        bool Owns(void *ptr);
    };
}