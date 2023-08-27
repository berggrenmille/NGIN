#include <NGIN/Memory/Mallocator.hpp>

namespace NGIN::Memory
{

    // Move assignment operator
    Mallocator &Mallocator::operator=(Mallocator &&other) noexcept
    {
        // No resources to move, so the body is empty
        return *this;
    }

    void *Mallocator::Allocate(size_t size,
                               size_t alignment,
                               const std::source_location &location)
    {
#if defined(_MSC_VER)
        return _aligned_malloc(size, alignment);
#else
        void *ptr;
        posix_memalign(&ptr, alignment, size);
        return ptr;
#endif
    }

    void Mallocator::Deallocate(void *ptr)
    {
#if defined(_MSC_VER)
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    bool Mallocator::Owns(void *ptr) const
    {
        return true; // Assume true, since Mallocator should be used as a fallback allocator
    }

} // namespace NGIN::Memory
