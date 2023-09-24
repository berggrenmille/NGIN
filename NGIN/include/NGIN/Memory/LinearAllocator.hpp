#pragma once
#include <NGIN/Defines.hpp>
#include "Internal/Alignment.hpp"

#include <memory>
#include <cstddef>
#include <source_location>

namespace NGIN::Memory
{

    class LinearAllocator
    {
    public:
        NGIN_API LinearAllocator(size_t size = 1024);

        NGIN_API LinearAllocator(LinearAllocator &&other) noexcept;

        NGIN_API LinearAllocator &operator=(LinearAllocator &&other) noexcept;

        NGIN_API void *Allocate(size_t size,
                                size_t alignment = alignof(std::max_align_t),
                                const std::source_location &location = std::source_location::current());

        NGIN_API void DeallocateAll();

        NGIN_API bool Owns(void *ptr) const;

        float test = 32;

    private:
        std::unique_ptr<std::byte[]> buffer;
        std::byte *currentPos;
        std::byte *end;
    };

} // namespace NGIN::Memory
