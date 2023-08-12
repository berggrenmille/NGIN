#pragma once
#include "Allocator.hpp"
#include <cstddef>
#include <new>

namespace NGIN
{
	/**
	 * @class FreeListAllocator
	 * @brief An allocator utilizing the free list strategy for memory management.
	 *
	 * The FreeListAllocator keeps track of free memory blocks using a linked list of free blocks.
	 * When a memory allocation is requested, it scans the list to find a suitable block.
	 * Once memory is deallocated, it gets reinserted into the free list.
	 *
	 * @note that there is an overhead for each allocation due to the AllocationHeader,
	 * which means that the actual memory consumed might be slightly more than the requested size.
	 */
	class NGIN_API FreeListAllocator : public Allocator
	{
	public:

		/**
		 * @brief Constructor to initialize the allocator with the specified total size.
		 *
		 * @param size The total size of memory managed by this allocator.
		 */
		explicit FreeListAllocator(size_t size);

		/**
		 * @brief Destructor that releases the managed memory.
		 */
		~FreeListAllocator();

		/**
		 * @brief Allocates memory of the requested size, ensuring proper alignment.
		 *
		 * @param size Size of the memory block to allocate.
		 * @param alignment Memory alignment requirement. Defaults to platform's max alignment.
		 * @param location Source code location for logging/debugging purposes. Defaults to the current location.
		 * @return Pointer to the allocated memory block, or nullptr if the allocation fails.
		 */
		virtual void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t),
							   const std::source_location& location = std::source_location::current()) override;

		/**
		 * @brief Deallocates the provided memory block and returns it to the free list.
		 *
		 * @param ptr Pointer to the memory block to be deallocated.
		 */
		virtual void Deallocate(void* ptr = nullptr) override;

		/**
		 * @brief Deallocates all managed memory blocks and resets the allocator.
		 */
		virtual void DeallocateAll() override;

	private:

		/**
		 * @brief Structure for the header information of each allocation.
		 *
		 * The AllocationHeader contains metadata about each memory block.
		 */
		struct AllocationHeader
		{
			size_t size;          ///< The size of the allocated memory.
			size_t adjustment;    ///< Adjustment for alignment.

			AllocationHeader(size_t size = 0, size_t adjustment = 0)
				: size(size), adjustment(adjustment)
			{}
		};

		/**
		 * @brief Structure for each free block in the list.
		 *
		 * FreeBlock represents a chunk of memory that is currently free and can be used
		 * for future allocations.
		 */
		struct FreeBlock
		{
			size_t size;          ///< Size of the free memory block.
			FreeBlock* previous;  ///< Pointer to the previous free block.
			FreeBlock* next;      ///< Pointer to the next free block.

			FreeBlock(size_t block_size = 0, FreeBlock* prev = nullptr, FreeBlock* next = nullptr)
				: size(block_size), previous(prev), next(next)
			{}
		};

		/// Minimum size of a block accounting for the largest of either a FreeBlock or an AllocationHeader plus one byte.
		static constexpr size_t minBlockSize = std::max(sizeof(FreeBlock), sizeof(AllocationHeader) + 1);

		/// Maximum alignment required by either a FreeBlock or an AllocationHeader.
		static constexpr size_t maxMetadataAlignment = std::max(alignof(FreeBlock), alignof(AllocationHeader));

		/**
		 * @brief Adds a free memory block to the free list.
		 *
		 * @param block Pointer to the block to be added to the free list.
		 */
		void AddBlockToFreeList(FreeBlock* block);

		/**
		 * @brief Removes a free memory block from the free list.
		 *
		 * @param block Pointer to the block to be removed from the free list.
		 */
		void RemoveBlockFromFreeList(FreeBlock* block);

		/**
		 * @brief Merges adjacent free blocks to form larger contiguous free blocks.
		 *
		 * @param block The block to start coalescing from.
		 */
		void CoalesceBlock(FreeBlock* block);

		/**
		 * @brief Finds a free block that satisfies the memory size and alignment requirements.
		 *
		 * @param size Size of the memory block to find.
		 * @param alignment Memory alignment requirement.
		 * @return Pair containing a pointer to the found free block and the alignment adjustment required.
		 */
		std::pair<FreeBlock*, size_t> FindFreeBlock(size_t size, size_t alignment);

		void* start;              ///< Start pointer of the memory managed by this allocator.
		FreeBlock* freeBlocks;    ///< Pointer to the start of the linked list of free blocks.
	};

} // namespace NGIN
