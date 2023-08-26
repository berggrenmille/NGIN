#pragma once
#include <cstddef>

namespace NGIN::Memory::Internal
{
    /**
     * @brief Aligns a pointer to the given alignment.
     *
     * @param ptr The pointer to align.
     * @param alignment The alignment to use.
     * @return The aligned pointer.
     */
    [[nodiscard]] inline void *AlignPtr(void *ptr, size_t alignment)
    {
        return reinterpret_cast<void *>((reinterpret_cast<size_t>(ptr) + (alignment - 1)) & ~(alignment - 1));
    }

    /**
     * @brief Returns true if the given pointer is aligned to the given alignment.
     *
     * @param ptr The pointer to check.
     * @param alignment The alignment to check.
     * @return True if the pointer is aligned, false otherwise.
     */
    [[nodiscard]] inline bool IsPtrAligned(void *ptr, size_t alignment)
    {
        return reinterpret_cast<size_t>(ptr) % alignment == 0;
    }

    /**
     * @brief Returns the number of bytes required to align the given pointer to the given alignment.
     *
     * @param ptr The pointer to align.
     * @param alignment The alignment to use.
     * @return The number of bytes required to align the pointer.
     */
    [[nodiscard]] inline size_t GetAlignmentPadding(void *ptr, size_t alignment)
    {
        return (alignment - (reinterpret_cast<size_t>(ptr) % alignment)) % alignment;
    }
}