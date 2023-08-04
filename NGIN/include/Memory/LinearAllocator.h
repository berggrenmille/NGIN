// LinearAllocator.h
#pragma once
#include <Memory/Allocator.h>

namespace NGIN
{
	/**
	 * @class LinearAllocator
	 * @brief Custom memory allocator that allocates memory linearly in a block (also known as an arena).
	 *
	 * A LinearAllocator is one of the simplest and fastest custom allocators. It allocates memory by
	 * incrementing a pointer within a pre-allocated block. Once the memory is exhausted, it can't be
	 * replenished. Memory deallocation can only happen when all allocations are deallocated at once.
	 */
	class LinearAllocator : public Allocator
	{
	public:
		/**
		 * @brief Constructor that initializes the LinearAllocator with a block of memory.
		 * @param size Size of the memory block to be used by the allocator.
		 */
		LinearAllocator(size_t size);

		/**
		 * @brief Destructor that frees the block of memory used by the allocator.
		 */
		~LinearAllocator();

		/**
		 * @brief Allocates a block of memory from the pre-allocated block.
		 * @param size Size of memory to allocate.
		 * @param alignment Alignment of the allocated memory. Default is max alignment.
		 * @return Pointer to the allocated memory, or nullptr if not enough memory remains.
		 */
		void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t)) override;

		/**
		 * @brief Deallocates a block of memory. In this case, does nothing.
		 * @note Deallocating individual blocks of memory is not supported by this allocator.
		 * @param ptr Pointer to the memory to deallocate. Default is nullptr.
		 */
		void Deallocate(void* ptr = nullptr) override;

		/**
		 * @brief Deallocates all memory by resetting the allocator.
		 *
		 * This is the only way to free memory in a LinearAllocator.
		 */
		void DeallocateAll() override;

	private:
		void* start;    ///< Start of pre-allocated memory block.
		void* current;  ///< Current allocation position within memory block.
		size_t size;    ///< Size of the pre-allocated memory block.
	};
}
