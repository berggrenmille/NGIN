#pragma once

#include <Core/Memory/Allocator.hpp>

namespace NGIN
{

	/**
	 * @class StackAllocator
	 * @brief Allocator that uses a stack-like LIFO strategy for memory allocation.
	 *
	 * The StackAllocator reserves a block of memory and allows for fast allocations by simply moving a marker.
	 * It's efficient but only allows deallocations in a reverse order.
	 */
	class NGIN_API StackAllocator : public Allocator
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
		virtual ~StackAllocator();

		virtual void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t),
							   const std::source_location& location = std::source_location::current()) override;

		/**
		 * @brief Deallocates the most recent block of memory.
		 *
		 * This function assumes that the provided pointer (@p ptr) corresponds to the most recent allocation.
		 * Providing a pointer to any other block or invalid memory results in undefined behavior. It is the
		 * responsibility of the caller to ensure that the pointer passed in is the last allocated block.
		 *
		 * @param ptr The pointer to the memory block to be deallocated.
		 */
		virtual void Deallocate(void* ptr = nullptr) override;

		virtual void DeallocateAll() override;

	private:
		void* startPtr;          ///< Pointer to the start of the memory block.
		void* currentTop;     ///< Current position or "top" of the stack.
	};

} // namespace NGIN

