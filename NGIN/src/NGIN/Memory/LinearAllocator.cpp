#include <NGIN/Memory/LinearAllocator.hpp>
#include <NGIN/Meta/Reflection/Registration.hpp>
namespace NGIN::Memory
{

    LinearAllocator::LinearAllocator(size_t size)
        : buffer(std::make_unique<std::byte[]>(size)), currentPos(buffer.get()), end(buffer.get() + size) {}

    LinearAllocator::LinearAllocator(LinearAllocator &&other) noexcept
        : buffer(std::move(other.buffer)), currentPos(other.currentPos), end(other.end)
    {
        other.currentPos = nullptr;
        other.end = nullptr;
    }

    LinearAllocator &LinearAllocator::operator=(LinearAllocator &&other) noexcept
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

    void *LinearAllocator::Allocate(size_t size,
                                    size_t alignment,
                                    const std::source_location &location)
    {
        const size_t padding = Internal::GetAlignmentPadding(currentPos, alignment);
        if (currentPos + padding + size > end)
            return nullptr;

        std::byte *alignedPos = currentPos + padding;
        currentPos = alignedPos + size;
        return alignedPos;
    }

    void LinearAllocator::DeallocateAll()
    {
        currentPos = buffer.get();
    }

    bool LinearAllocator::Owns(void *ptr) const
    {
        uintptr_t startAddress = reinterpret_cast<uintptr_t>(buffer.get());
        uintptr_t endAddress = reinterpret_cast<uintptr_t>(end);
        uintptr_t targetAddress = reinterpret_cast<uintptr_t>(ptr);

        return targetAddress >= startAddress && targetAddress < endAddress;
    }
} // namespace NGIN::Memory

NGIN_REFLECTION_REGISTRATION()
{
    using namespace NGIN::Meta::Reflection;
    RegisterClass<NGIN::Memory::LinearAllocator>().RegisterMethod("DeallocateAll", &NGIN::Memory::LinearAllocator::DeallocateAll);
}