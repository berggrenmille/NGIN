#include <Memory/Allocator.h>
#include <Logger.h>
namespace NGIN
{

	Allocator::Allocator()
	{
#ifdef DEBUG
		debugAllocations.reserve(100);
#endif // DEBUG
	}

	Allocator::~Allocator()
	{
#ifdef DEBUG
		for (auto& handle : debugAllocations)
		{
			if (handle.ptr)
			{


				Logger::Log(handle.location, Logger::Verbosity::ERROR, "Memory not freed at of size %d bytes at %p", handle.size, handle.ptr);
			}
		}
#endif // DEBUG
	}

#ifdef DEBUG
	void Allocator::AddDebugAllocation(void* ptr, size_t size, const std::source_location& location)
	{
		debugAllocations.push_back({ ptr, size, location });
	}
#endif // DEBUG

	uintptr_t Allocator::GetAlignmentOffset(size_t alignment, const void* const ptr)
	{
		return (uintptr_t)(ptr) & (alignment - 1);
	}

	uintptr_t Allocator::GetAlignmentAdjustment(size_t alignment, const void* const ptr)
	{
		uintptr_t offset = GetAlignmentOffset(alignment, ptr);
		if (offset == 0)
			return 0;
		return alignment - offset;

	}
}