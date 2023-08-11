#pragma once
#include <Memory/Allocator.h>
#include <mutex>

namespace NGIN
{

	/**
	 * @class LinearAllocator
	 * @brief A memory allocator that allocates memory in a linear fashion.
	 *
	 * The LinearAllocator uses a single contiguous block of memory and places each new allocation directly after the last.
	 * It does not support deallocation of individual objects. Instead, all the memory can be deallocated at once.
	 * This allocator is designed to be fast and is thread-safe.
	 *
	 * @note The allocator does not support deallocation of individual objects, but all objects can be deallocated at once.
	 */
	class NGIN_API LinearAllocator : public Allocator
	{
	public:
		/**
		 * @brief Constructor for LinearAllocator.
		 *
		 * @param size The size of the memory block that the allocator manages.
		 */
		explicit LinearAllocator(size_t size);

		/**
		 * @brief Destructor for LinearAllocator.
		 *
		 * Frees the memory block used by the allocator.
		 */
		~LinearAllocator();

		void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t),
					   const std::source_location& location = std::source_location::current()) override;

		/**
		* @brief Deallocate is not supported in LinearAllocator
		* Use DeallocateAll() to reset the allocator instead.
		*/
		void Deallocate(void* ptr = nullptr) override;

		void DeallocateAll() override;

		/**
		 * @brief Gets the remaining memory of the allocator.
		 *
		 * @return Remaining memory in bytes.
		 */
		size_t getRemainingMemory() const;

	private:
		void* startPtr;      ///< Start of the memory block.
		void* currentPtr;    ///< Current position in the block.
	};

}  // namespace NGIN
