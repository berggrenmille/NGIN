#include <Memory/LinearAllocator.h>

namespace NGIN
{

	LinearAllocator::LinearAllocator(size_t size)
		: start_(static_cast<uint8_t*>(std::malloc(size))), offset_(0), size_(size)
	{}

	LinearAllocator::~LinearAllocator()
	{
		std::free(start_);
	}

	void* LinearAllocator::Allocate(size_t size, size_t alignment)
	{
		uintptr_t rawAddress = reinterpret_cast<uintptr_t>(start_) + offset_;
		uintptr_t offsetAdjustment = GetAlignmentAdjustment(alignment, (void*)rawAddress);
		if (offset_ + offsetAdjustment + size > size_) return nullptr; // Not enough memory

		offset_ += offsetAdjustment + size;

#ifdef DEBUG
		// create and store debug handle
		DebugAllocatorHandle handle = { reinterpret_cast<void*>(rawAddress + offsetAdjustment), size, std::source_location::current() };
		debugAllocations.push_back(handle);
#endif

		return reinterpret_cast<void*>(rawAddress + offsetAdjustment);
	}

	void LinearAllocator::Deallocate(void* ptr)
	{
		// Linear allocator does not support deallocation of individual blocks
	}

	void LinearAllocator::DeallocateAll()
	{
#ifdef DEBUG
		debugAllocations.clear();
#endif
		offset_ = 0;
	}
}
