#pragma once
#include <NGIN/Core.h>
#include "Internal/Alignment.hpp"

#include <memory>
#include <cstddef>
#include <source_location>

namespace NGIN::Memory
{

    class NGIN_API LinearAllocator
    {
    public:
        LinearAllocator(size_t size);

        LinearAllocator(LinearAllocator &&other) noexcept;

        LinearAllocator &operator=(LinearAllocator &&other) noexcept;

        void *Allocate(size_t size,
                       size_t alignment = alignof(std::max_align_t),
                       const std::source_location &location = std::source_location::current());

        void Deallocate(void *ptr);

        void DeallocateAll();

    private:
        std::unique_ptr<std::byte[]> buffer;
        std::byte *currentPos;
        std::byte *end;
    };

} // namespace NGIN::Memory
