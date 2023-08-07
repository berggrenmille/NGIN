#pragma once
#include <Core/Core.h>

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <concepts>
#include <source_location>
#include <vector>
namespace NGIN
{

	/**
	 * @class Allocator
	 * @brief A base class for custom memory allocation.
	 *
	 * This class provides an interface for memory allocation and deallocation, and allows
	 * derived classes to provide custom allocation strategies. It also provides debug information
	 * for detecting memory leaks.
	 */
	class Allocator
	{
#ifdef DEBUG
	protected:
		struct DebugAllocatorHandle
		{
			void* ptr;
			size_t size;
			std::source_location location;
			friend bool operator==(const DebugAllocatorHandle& lhs, const DebugAllocatorHandle& rhs) { return lhs.ptr == rhs.ptr && lhs.size == rhs.size; };
		};

		std::vector<DebugAllocatorHandle> debugAllocations;

		void AddDebugAllocation(void* ptr, size_t size, const std::source_location& location = std::source_location::current());
#endif // DEBUG
	public:
		/// Default constructor.
		Allocator();

		/**
		* @brief Copy constructor marked as delete to prevent copying.
		*
		* This is done because memory allocated by one instance of the allocator
		* should not be shared or managed by another instance.
		*/
		Allocator(const Allocator&) = delete;

		/**
		 * @brief Assignment operator marked as delete to prevent assignment.
		 *
		 * This is done because memory allocated by one instance of the allocator
		 * should not be shared or managed by another instance.
		 */
		Allocator& operator=(const Allocator&) = delete;

		///destructor
		virtual ~Allocator();

		/**
		 * @brief Allocates a block of memory.
		 *
		 * Allocates a block of memory of the specified size, with the specified alignment. The actual
		 * allocation strategy is provided by the derived classes.
		 *
		 * @param size Size of memory to allocate.
		 * @param alignment Alignment of the allocated memory. Default is max alignment.
		 * @return Pointer to the allocated memory.
		 */
		virtual void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t)) = 0;

		/**
		 * @brief Deallocates a block of memory.
		 *
		 * Deallocates the block of memory pointed to by the specified pointer. The actual
		 * deallocation strategy is provided by the derived classes.
		 *
		 * @param ptr Pointer to the memory to deallocate. Default is nullptr.
		 */
		virtual void Deallocate(void* ptr = nullptr) = 0;

		/**
		 * @brief Deallocates all blocks of memory
		 *
		 * Deallocates all blocks of memory reserved by the allocator. The actual
		 * deallocation strategy is provided by the derived classes.
		 *
		 * @param ptr Pointer to the memory to deallocate. Default is nullptr.
		 */
		virtual void DeallocateAll() = 0;

		/**
		 * @brief Constructs an object in the allocated memory.
		 *
		 * Constructs an object of the specified type in the memory allocated by the Allocate method.
		 * The constructor of the object is called with the specified arguments.
		 *
		 * @tparam T Type of object to construct.
		 * @tparam Args Types of constructor arguments.
		 * @param args Arguments to pass to the constructor of T.
		 * @return Pointer to the constructed object.
		 */
		template<class T, class... Args>
		T* New(Args&&... args, const std::source_location& location = std::source_location::current());

		/**
		 * @brief Destroys an object and deallocates its memory.
		 *
		 * Calls the destructor of the specified object and then deallocates the memory for the object.
		 *
		 * @tparam T Type of the object.
		 * @param object Reference to the object to destroy.
		 */
		template<class T>
		void Delete(T* object, const std::source_location& location = std::source_location::current());
	protected:

		/**
		 * @brief Get the alignment offset of a pointer.
		 *
		 * Returns the offset from the desired alignment for a given pointer. Assumes
		 * alignment is a power of 2.
		 *
		 * @param alignment Desired alignment. Must be a power of 2.
		 * @param ptr Pointer to get the offset of.
		 * @return Offset needed to achieve desired alignment.
		 */
		uintptr_t GetAlignmentOffset(size_t alignment, const void* const ptr);

		/**
		 * @brief Get the alignment adjustment of a pointer.
		 *
		 * Returns the adjustment needed to align a pointer to the desired alignment. Assumes
		 * alignment is a power of 2.
		 *
		 * @param alignment Desired alignment. Must be a power of 2.
		 * @param ptr Pointer to get the adjustment of.
		 * @return Adjustment needed to achieve desired alignment.
		 */
		uintptr_t GetAlignmentAdjustment(size_t alignment, const void* const ptr);


	};

	/**
	 * @brief Checks if a type is an allocator.
	 *
	 * @tparam T Type to check.
	 */
	template<class T>
	concept is_allocator = std::is_base_of<Allocator, T>::value;

	/**
	 * @brief Checks if a type is a pointer to an allocator.
	 *
	 * @tparam T Type to check.
	 */
	template<typename T>
	concept is_allocator_ptr = std::is_base_of<Allocator, std::remove_pointer_t<T>>::value;
}

#ifdef DEBUG 

#define NGIN_NEW(allocator, type, ...) allocator.New<type>(__VA_ARGS__, std::source_location::current())
#define NGIN_DELETE(allocator, object) allocator.Delete(object, std::source_location::current())


#endif DEBUG
#include <Memory/Allocator.inl>
