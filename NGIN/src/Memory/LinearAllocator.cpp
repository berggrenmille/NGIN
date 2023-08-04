// LinearAllocator.cpp
#include <Memory/LinearAllocator.h>
#include <Logger.h>

namespace NGIN
{
	LinearAllocator::LinearAllocator(size_t size) : size(size)
	{
		// Allocate a block of memory of the specified size
		start = std::malloc(size);
		// Set the current position to the start of the memory block
		current = start;
	}

	LinearAllocator::~LinearAllocator()
	{
		// Free the block of memory
		std::free(start);
	}

	void* LinearAllocator::Allocate(size_t size, size_t alignment)
	{
		// Calculate the adjustment needed to keep the object correctly aligned
		uintptr_t adjustment = GetAlignmentAdjustment(alignment, current);

		// Check if there's enough remaining memory
		if ((uintptr_t)current + adjustment + size > (uintptr_t)start + this->size)
			return nullptr; // Not enough memory

		// Determine the new memory location
		void* alignedAddress = (void*)((uintptr_t)current + adjustment);

		// Update the current position to the next free memory location
		current = (void*)((uintptr_t)alignedAddress + size);

		// Return the allocated memory
		return alignedAddress;
	}

	void LinearAllocator::Deallocate(void* ptr)
	{
		// Deallocate does nothing in a linear allocator. This is because in a linear allocator, 
		// freeing individual objects isn't possible without tracking the size of each allocation. 
		// If deallocation of individual objects is required, consider using a different type of allocator.
		NGIN_ASSERT(false, "Deallocate is not supported in a linear allocator");
	}

	void LinearAllocator::DeallocateAll()
	{
		// Reset the current position to the start of the memory block
		// This effectively deallocates all the memory, since we will start overwriting
		// the old objects on the next allocation
		current = start;
	}
}