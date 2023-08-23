#pragma once
#include <NGIN/Core.h>
#include "Internal/Alignment.hpp"

#include <memory>
#include <cstddef>
#include <source_location>

namespace NGIN::Memory
{

    class LinearAllocator
    {
    public:
        /**
         * @brief Constructs a LinearAllocator with a given size.
         *
         * The LinearAllocator pre-allocates a buffer of the given size. Memory is allocated
         * from this buffer in a linear fashion.
         *
         * @param size Size of the pre-allocated buffer.
         */
        LinearAllocator(size_t size)
            : buffer(std::make_unique<std::byte[]>(size)), currentPos(buffer.get()), end(buffer.get() + size) {}

        // Move constructor
        LinearAllocator(LinearAllocator &&other) noexcept
            : buffer(std::move(other.buffer)), currentPos(other.currentPos), end(other.end)
        {
            other.currentPos = nullptr;
            other.end = nullptr;
        }

        // Move assignment operator
        LinearAllocator &operator=(LinearAllocator &&other) noexcept
        {
            if (this != &other)
            {
                buffer = std::move(other.buffer);
                currentPos = other.currentPos;
                end = other.end;

                other.currentPos = nullptr;
                other.end = nullptr;
            }
            return *this;
        }

        /**
         * @brief Allocates a block of memory of a given size and alignment.
         *
         * Memory is allocated linearly from the pre-allocated buffer.
         *
         * @param size Size of the memory block to allocate.
         * @param alignment Memory alignment requirement.
         * @param location Source location for potential debugging purposes (unused in LinearAllocator).
         * @return Pointer to the allocated block, or nullptr if allocation fails.
         */
        void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t), const std::source_location &location = std::source_location::current())
        {
            const size_t padding = Internal::GetAlignmentPadding(currentPos, alignment);
            if (currentPos + padding + size > end)
                return nullptr; // not enough space

            char *alignedPos = currentPos + padding;
            currentPos = alignedPos + size;
            return alignedPos;
        }

        /**
         * @brief Deallocates a memory block.
         *
         * Note: Individual deallocation is not supported by LinearAllocator. This function is a no-op.
         *
         * @param ptr Pointer to the memory block to deallocate (unused).
         */
        void Deallocate(void *ptr)
        {
            // No-op: LinearAllocator doesn't support individual deallocation
        }

        /**
         * @brief Resets the allocator.
         *
         * This deallocates all previously allocated memory blocks and allows the entire buffer to be reused.
         */
        void DeallocateAll()
        {
            currentPos = buffer.get();
        }

    private:
        std::unique_ptr<char[]> buffer; ///< The pre-allocated buffer from which memory is allocated.
        char *currentPos;               ///< Current position in the buffer. Next allocation starts here.
        char *end;                      ///< Points to one past the last byte of the buffer.
    };

} // namespace NGIN::Memory
