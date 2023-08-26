#pragma once

#include <NGIN/Core.h>

namespace NGIN::Memory
{

	/**
	 * @class StackAllocator
	 * @brief Allocator that uses a stack-like LIFO strategy for memory allocation.
	 *
	 * The StackAllocator reserves a block of memory and allows for fast allocations by simply moving a marker.
	 * It's efficient but only allows deallocations in a reverse order.
	 */
	class NGIN_API StackAllocator
	{
	public:
		/**
		 * @brief Constructor to initialize the stack allocator with a given size.
		 *
		 * @param size The total size of memory to reserve for this allocator.
		 */
		explicit StackAllocator(size_t size);

		/**
		 * @brief Default destructor.
		 */
		~StackAllocator();

		StackAllocator(const StackAllocator &) = delete;

		StackAllocator(StackAllocator &&other) noexcept;

		void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t),
					   const std::source_location &location = std::source_location::current());

		/**
		 * @brief Deallocates the most recent block of memory.
		 *
		 * This function assumes that the provided pointer (@p ptr) corresponds to the most recent allocation.
		 * Providing a pointer to any other block or invalid memory results in undefined behavior. It is the
		 * responsibility of the caller to ensure that the pointer passed in is the last allocated block.
		 *
		 * @param ptr The pointer to the memory block to be deallocated.
		 */
		void Deallocate(void *ptr = nullptr);

		void DeallocateAll();

		bool Owns(void *ptr) const;

	private:
		void *startPtr;	   ///< Pointer to the start of the memory block.
		void *currentTop;  ///< Current position or "top" of the stack.
		size_t usedMemory; ///< The amount of memory currently used.
		size_t size;	   ///< The total size of the memory block.
	};

} // namespace NGIN
