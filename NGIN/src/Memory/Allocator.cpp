#include <PCH.h>
#include <Memory/Allocator.h>
#include <Core/Logger.h>
namespace NGIN
{

	Allocator::Allocator()
		: size(0), usedMemory(0)
	{
#ifdef NGIN_DEBUG
		debugAllocations.reserve(100);
#endif // DEBUG
	}

	Allocator::~Allocator()
	{
#ifdef NGIN_DEBUG
		for (auto& handle : debugAllocations)
		{
			if (handle.ptr)
			{
				Logger::Log(handle.location, Logger::Verbosity::ERROR,
							"Object of type {} never got destroyed at {}", handle.typeName, handle.ptr);  // updated to show type name
			}
		}
#endif
	}

#ifdef NGIN_DEBUG
	void Allocator::AddDebugAllocation(void* ptr, const std::string& typeName, const std::source_location& location)
	{
		debugAllocations.push_back({ ptr, typeName, location });   // added typeName here
	}
#endif

	Allocator::Address Allocator::GetAlignmentOffset(size_t alignment, const void* const ptr)
	{
		return (Address)(ptr) & (alignment - 1);
	}

	Allocator::Address Allocator::GetAlignmentAdjustment(size_t alignment, const void* const ptr)
	{
		Address offset = GetAlignmentOffset(alignment, ptr);
		if (offset == 0)
			return 0;
		return alignment - offset;

	}
}