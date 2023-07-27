#pragma once
#include "Allocator.h"

namespace NGIN
{

	class LinearAllocator : public Allocator
	{
	public:
		LinearAllocator(size_t size);
		~LinearAllocator();

		virtual void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t)) override;
		virtual void Deallocate(void* ptr = nullptr) override;
		virtual void DeallocateAll() override;

	private:
		uint8_t* start_;
		size_t offset_;
		size_t size_;
	};
}
