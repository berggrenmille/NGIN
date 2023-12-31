#include <PCH.h>
#include <NGIN/Memory/StackAllocator.hpp>
#include <NGIN/Memory/Internal/Alignment.hpp>

namespace NGIN::Memory
{

	StackAllocator::StackAllocator(size_t size)
		: startPtr(::operator new(size))
	{
		// Might be able to handle this error more gracefully, like throwing an exception or falling back to another allocator
		// Very unlikely, soooo... TODO
		NGIN_ASSERT(startPtr != nullptr, "StackAllocator failed to allocate memory at contruction");

		this->size = size;
		this->usedMemory = 0;
		currentTop = startPtr;
	}

	StackAllocator::~StackAllocator()
	{
		::operator delete(startPtr);
	}

	StackAllocator::StackAllocator(StackAllocator &&other) noexcept
		: startPtr(other.startPtr), currentTop(other.currentTop),
		  usedMemory(other.usedMemory), size(other.size)
	{
		// Invalidate the original allocator to prevent double-deletion of the memory.
		other.startPtr = nullptr;
		other.currentTop = nullptr;
		other.usedMemory = 0;
		other.size = 0;
	}

	void *StackAllocator::Allocate(size_t size, size_t alignment, const std::source_location &location)
	{
		uintptr_t currentAddress = reinterpret_cast<uintptr_t>(currentTop);
		uintptr_t adjustment = Internal::GetAlignmentPadding(currentTop, alignment);

		// Check if there's enough memory left to allocate the required amount
		if (usedMemory + adjustment + size > this->size)
		{
			return nullptr; // Not enough memory
		}

		// Adjust currentTop to the aligned memory address
		currentTop = reinterpret_cast<void *>(currentAddress + adjustment);

		void *alignedMemory = currentTop;
		currentTop = reinterpret_cast<void *>(currentAddress + adjustment + size);

		usedMemory += adjustment + size;

		return alignedMemory;
	}

	void StackAllocator::Deallocate(void *ptr)
	{
		// In a stack allocator, you can't deallocate any random block.
		// We'll assume that we only deallocate the most recent block.
		// If more flexibility is needed, consider using a different allocator

		usedMemory -= reinterpret_cast<uintptr_t>(currentTop) - reinterpret_cast<uintptr_t>(ptr);
		currentTop = ptr;
	}

	void StackAllocator::DeallocateAll()
	{
		currentTop = startPtr;
		usedMemory = 0;
	}

	bool StackAllocator::Owns(void *ptr)
	{
		uintptr_t startAddress = reinterpret_cast<uintptr_t>(startPtr);
		uintptr_t endAddress = reinterpret_cast<uintptr_t>(currentTop); // Assuming currentTop points to the current end of used memory
		uintptr_t targetAddress = reinterpret_cast<uintptr_t>(ptr);

		return targetAddress >= startAddress && targetAddress < endAddress;
	}

} // namespace NGIN
