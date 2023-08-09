#pragma once
#include <Memory/Allocator.h>
#include <cstddef>
#include <new>

namespace NGIN
{

	/**
	 * @class FreeListAllocator
	 * @brief Allocator that manages memory using a free list strategy.
	 *
	 * The FreeListAllocator maintains a list of free blocks. When an allocation request
	 * is made, it searches the list for a suitable block. On deallocation, the block
	 * is added back to the free list.
	 */
	class NGIN_API FreeListAllocator : public Allocator
	{
	public:
		/**
		 * @brief Initializes the allocator with a given size.
		 *
		 * @param size The total size of memory to be managed by this allocator.
		 */
		explicit FreeListAllocator(size_t size);

		/**
		 * @brief Default destructor. Releases managed memory.
		 */
		~FreeListAllocator();

		/**
		 * @brief Allocates a block of memory of the specified size.
		 *
		 * @param size The size of memory to allocate.
		 * @param alignment The alignment requirement.
		 * @return Pointer to the allocated memory or nullptr if allocation fails.
		 */
		virtual void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t),
							   const std::source_location& location = std::source_location::current()) override;

		/**
		 * @brief Deallocates a block of memory and adds it back to the free list.
		 *
		 * @param ptr Pointer to the memory block to deallocate.
		 */
		virtual void Deallocate(void* ptr = nullptr) override;
		virtual void DeallocateAll() override;

	private:
		struct AllocationHeader
		{
			size_t size;
			size_t adjustment;
			AllocationHeader(size_t size = 0, size_t adjustment = 0) : adjustment(adjustment), size(size)
			{}
		};

		struct FreeBlock
		{
			size_t size;
			FreeBlock* previous;
			FreeBlock* next;

			FreeBlock(size_t block_size = 0, FreeBlock* prev = nullptr, FreeBlock* next = nullptr)
				: size(block_size), previous(prev), next(next)
			{}
		};

		static constexpr size_t minBlockSize =
			std::max(sizeof(FreeBlock), sizeof(AllocationHeader) + 1);

		static constexpr size_t maxMetadataAlignment =
			std::max(alignof(FreeBlock), alignof(AllocationHeader));

		void AddBlockToFreeList(FreeBlock* block);
		void RemoveBlockFromFreeList(FreeBlock* block);
		void CoalesceBlock(FreeBlock* block);
		std::pair<FreeBlock*, size_t> FindFreeBlock(size_t size, size_t alignment);

		void* start;
		FreeBlock* freeBlocks;  ///< Pointer to the head of the free list.
	};



} // namespace NGIN
