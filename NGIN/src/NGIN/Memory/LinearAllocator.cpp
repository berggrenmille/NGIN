#include <PCH.h>
#include <NGIN/Memory/LinearAllocator.hpp>
#include <cassert>

namespace NGIN
{

	LinearAllocator::LinearAllocator(size_t size)
		: startPtr(::operator new(size)), currentPtr(startPtr)
	{
		this->size = size;
		this->usedMemory = 0;
		NGIN_ASSERT(size > 0, "Size must be greater than zero");
	}

	LinearAllocator::~LinearAllocator()
	{

		::operator delete(startPtr);
	}

	void* LinearAllocator::Allocate(size_t size, size_t alignment, const std::source_location& location)
	{


		uintptr_t adjustment = GetAlignmentAdjustment(alignment, currentPtr);

		if (usedMemory + adjustment + size > this->size)
		{
			Logger::Log(location, Logger::Verbosity::ERROR, "LinearAllocator is out of memory");
			return nullptr;  // Out of memory
		}
		uintptr_t alignedAddress = (uintptr_t)currentPtr + adjustment;

		// Update bookkeeping
		usedMemory += size + adjustment;
		currentPtr = (void*)(alignedAddress + size);

		return (void*)alignedAddress;
	}

	void LinearAllocator::Deallocate(void* ptr)
	{
		// Linear allocator does not support deallocation. Use DeallocateAll().
	}

	void LinearAllocator::DeallocateAll()
	{


		// Reset pointers
		currentPtr = startPtr;
		usedMemory = 0;


#ifdef NGIN_DEBUG
		ClearDebugAllocations();
#endif
	}

	size_t LinearAllocator::getRemainingMemory() const
	{
		return size - usedMemory;
	}

}  // namespace NGIN
