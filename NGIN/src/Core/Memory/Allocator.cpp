#include <PCH.h>
#include <Core/Memory/Allocator.hpp>
#include <Core/Logger.h>
namespace NGIN
{
#ifdef NGIN_DEBUG
	struct Allocator::DebugAllocationHandle
	{
		void* ptr;
		std::string typeName;
		std::source_location location;
		friend bool operator==(const DebugAllocationHandle& lhs, const DebugAllocationHandle& rhs) { return lhs.ptr == rhs.ptr; };
	};

	struct Allocator::DebugImpl
	{
		std::vector<DebugAllocationHandle> allocations;
	};
#endif
	Allocator::Allocator()
		: size(0), usedMemory(0)
	{

#ifdef NGIN_DEBUG
		debugImpl = new DebugImpl();
		debugImpl->allocations.reserve(100);
#endif // DEBUG
	}

	Allocator::~Allocator()
	{
#ifdef NGIN_DEBUG
		for (auto& handle : debugImpl->allocations)
		{
			if (handle.ptr)
			{
				Logger::Log(handle.location, Logger::Verbosity::ERROR,
							"Object of type {} never got destroyed at {}", handle.typeName, handle.ptr);  // updated to show type name
			}
		}
		delete debugImpl;
#endif
	}

#ifdef NGIN_DEBUG
	void Allocator::AddDebugAllocation(void* ptr, const std::string& typeName, const std::source_location& location)
	{
		debugImpl->allocations.push_back({ ptr, typeName, location });
	}
	void Allocator::RemoveDebugAllocation(void* ptr)
	{
		auto it = std::find_if(debugImpl->allocations.begin(), debugImpl->allocations.end(),
							   [&ptr](const DebugAllocationHandle& handle) { return handle.ptr == ptr; });
		if (it != debugImpl->allocations.end())
		{
			debugImpl->allocations.erase(it);
		}
	}
	void Allocator::ClearDebugAllocations()
	{
		debugImpl->allocations.clear();
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