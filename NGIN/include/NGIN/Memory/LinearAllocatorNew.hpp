#pragma once
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
            : m_buffer(std::make_unique<char[]>(size)), m_currentPos(m_buffer.get()), m_end(m_buffer.get() + size)
        {
        }

        // Move constructor
        LinearAllocator(LinearAllocator &&other) noexcept
            : m_buffer(std::move(other.m_buffer)),
              m_currentPos(other.m_currentPos),
              m_end(other.m_end)
        {
            other.m_currentPos = nullptr;
            other.m_end = nullptr;
        }

        // Move assignment operator
        LinearAllocator &operator=(LinearAllocator &&other) noexcept
        {
            if (this != &other)
            {
                m_buffer = std::move(other.m_buffer);
                m_currentPos = other.m_currentPos;
                m_end = other.m_end;

                other.m_currentPos = nullptr;
                other.m_end = nullptr;
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
            void *ptr = m_currentPos;
            size_t ptr2 = m_end - m_currentPos;
            char *alignedPos = reinterpret_cast<char *>(std::align(alignment, size, ptr, ptr2));
            if (!alignedPos)
                return nullptr; // not enough space

            m_currentPos = alignedPos + size;
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
            // No-op: individual deallocation is not supported
        }

        /**
         * @brief Resets the allocator.
         *
         * This deallocates all previously allocated memory blocks and allows the entire buffer to be reused.
         */
        void DeallocateAll()
        {
            m_currentPos = m_buffer.get();
        }

    private:
        std::unique_ptr<char[]> m_buffer; ///< The pre-allocated buffer from which memory is allocated.
        char *m_currentPos;               ///< Current position in the buffer. Next allocation starts here.
        char *m_end;                      ///< Points to one past the last byte of the buffer.
    };

} // namespace NGIN::Memory
