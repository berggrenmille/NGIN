#include <Memory/Allocator.h>

namespace NGIN
{

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