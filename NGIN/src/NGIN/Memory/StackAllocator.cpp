#include <PCH.h>
#include <NGIN/Memory/StackAllocator.hpp>


namespace NGIN
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

	void* StackAllocator::Allocate(size_t size, size_t alignment, const std::source_location& location)
	{
		uintptr_t currentAddress = reinterpret_cast<uintptr_t>(currentTop);
		uintptr_t adjustment = GetAlignmentAdjustment(alignment, currentTop);

		// Check if there's enough memory left to allocate the required amount
		if (usedMemory + adjustment + size > this->size)
		{
			return nullptr; // Not enough memory
		}

		// Adjust currentTop to the aligned memory address
		currentTop = reinterpret_cast<void*>(currentAddress + adjustment);

		void* alignedMemory = currentTop;
		currentTop = reinterpret_cast<void*>(currentAddress + adjustment + size);

		usedMemory += adjustment + size;

		return alignedMemory;
	}

	void StackAllocator::Deallocate(void* ptr)
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

#ifdef NGIN_DEBUG

		ClearDebugAllocations();
#endif
	}

} // namespace NGIN
