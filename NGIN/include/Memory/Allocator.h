#pragma once
#include <Core.h>
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
	* @brief A base class for memory allocation.
	*
	* This class represents a basic allocator for memory management.
	* It provides an interface for memory allocation and deallocation.
	*/
	class Allocator
	{
	protected:
#ifdef DEBUG
		struct DebugAllocatorHandle
		{
			void* ptr;
			size_t size;
			std::source_location& location;
			friend bool operator==(const DebugAllocatorHandle& lhs, const DebugAllocatorHandle& rhs) { return lhs.ptr == rhs.ptr && lhs.size == rhs.size; };
		};

		static std::vector<DebugAllocatorHandle> debugAllocations;
#endif // DEBUG
	public:
		/// Default constructor.
		Allocator() = default;

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
		virtual ~Allocator() {}

		/**
		 * @brief Allocates a block of memory.
		 *
		 * @param size Size of memory to allocate.
		 * @param alignment Alignment of the allocated memory. Default is max alignment.
		 * @return Pointer to the allocated memory.
		 */
		virtual void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t)) = 0;

		/**
		 * @brief Deallocates a block of memory.
		 *
		 * @param ptr Pointer to the memory to deallocate. Default is nullptr.
		 */
		virtual void Deallocate(void* ptr = nullptr) = 0;

		/// Deallocates all memory managed by the allocator.
		virtual void DeallocateAll() = 0;

		/**
		 * @brief Constructs an object in the allocated memory.
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
		 * @tparam T Type of the object.
		 * @param object Reference to the object to destroy.
		 */
		template<class T>
		void Delete(T* object, const std::source_location& location = std::source_location::current());
	protected:

		/**
		 * @brief Get the alignment offset of a pointer.
		 *
		 * @param alignment Desired alignment.
		 * @param ptr Pointer to get the offset of.
		 * @return Offset needed to achieve desired alignment.
		 */
		uintptr_t GetAlignmentOffset(size_t alignment, const void* const ptr);

		/**
		 * @brief Get the alignment adjustment of a pointer.
		 *
		 * @param alignment Desired alignment.
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
#include <Memory/Allocator.inl>
