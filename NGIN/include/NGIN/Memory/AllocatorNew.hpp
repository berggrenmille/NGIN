#pragma once
#include <NGIN/Core.h>

#include <cstddef>
#include <type_traits>
#include <utility>
#include <concepts>
#include <source_location>
#include <memory>

namespace NGIN::Memory
{
    /**
     * @brief Checks if a type satisfies the required interface for an allocator.
     *
     * This concept ensures any given type provides methods for allocation,
     * deallocation, and bulk deallocation.
     */
    template <typename T>
    concept AllocatorConcept = requires(T alloc, size_t size, size_t alignment, const std::source_location &location, void *ptr) {
        {
            alloc.Allocate(size, alignment, location)
        } -> std::same_as<void *>;
        {
            alloc.Deallocate(ptr)
        } -> std::same_as<void>;
        {
            alloc.DeallocateAll()
        } -> std::same_as<void>;
        // Extend this if you have more methods.
    };

    /**
     * @brief A type-erased allocator class that abstracts different allocator types behind a common interface.
     *
     * The Allocator class utilizes type erasure and manual virtual dispatch (MVD) to achieve
     * polymorphic behavior without the typical overhead of virtual function tables.
     *
     * \todo Implement Small Buffer Optimization (SBO).
     */
    class Allocator
    {
    public:
        using AllocateFn = void *(*)(void *, size_t, size_t, const std::source_location &);
        using DeallocateFn = void (*)(void *, void *);
        using DeallocateAllFn = void (*)(void *);

        /**
         * @brief Constructs the Allocator using a specific allocator type.
         *
         * The given allocator type is wrapped using type erasure. Manual virtual dispatch is employed
         * by setting up function pointers for each operation.
         *
         * @param alloc The allocator instance to be wrapped.
         */
        template <AllocatorConcept AllocatorT>
        Allocator(AllocatorT &&alloc)
            : pimpl(std::make_unique<Model<AllocatorT>>(std::forward<AllocatorT>(alloc)))
        {
            allocateFn = [](void *self, size_t size, size_t alignment, const std::source_location &location)
            {
                return static_cast<Model<AllocatorT> *>(self)->Allocate(size, alignment, location);
            };

            deallocateFn = [](void *self, void *ptr)
            {
                return static_cast<Model<AllocatorT> *>(self)->Deallocate(ptr);
            };

            deallocateAllFn = [](void *self)
            {
                return static_cast<Model<AllocatorT> *>(self)->DeallocateAll();
            };
        }

        /**
         * @brief Allocates memory using the wrapped allocator.
         *
         * @param size Size of the memory block to allocate.
         * @param alignment Memory alignment requirement (default to max alignment).
         * @param location Source location for potential debugging purposes.
         * @return Pointer to the allocated block.
         */
        void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t), const std::source_location &location = std::source_location::current()) const
        {
            return allocateFn(pimpl.get(), size, alignment, location);
        }

        /**
         * @brief Deallocates a memory block using the wrapped allocator.
         *
         * @param ptr Pointer to the memory block to deallocate.
         */

        void Deallocate(void *ptr) const
        {
            deallocateFn(pimpl.get(), ptr);
        }

        /**
         * @brief Deallocates all memory blocks managed by the wrapped allocator.
         */
        void DeallocateAll() const
        {
            deallocateAllFn(pimpl.get());
        }

        // Extend this if you have more methods.

    private:
        /**
         * @brief Wrapper around a specific allocator type.
         *
         * This struct holds an instance of the wrapped allocator and provides the required
         * interface using member functions.
         *
         * @tparam AllocatorT Type of the allocator being wrapped.
         */
        template <AllocatorConcept AllocatorT>
        struct Model
        {
            explicit Model(AllocatorT allocator)
                : allocator(std::move(allocator))
            {
            }

            void *Allocate(size_t size, size_t alignment, const std::source_location &location) const
            {
                return allocator.Allocate(size, alignment, location);
            }

            void Deallocate(void *ptr) const
            {
                allocator.Deallocate(ptr);
            }

            void DeallocateAll() const
            {
                allocator.DeallocateAll();
            }

            // Extend this if you have more methods.

        private:
            AllocatorT allocator; ///< Underlying instance of the wrapped allocator.
        };

        std::unique_ptr<void> pimpl; ///< Underlying instance of the wrapped allocator.

        AllocateFn allocateFn;           ///< Function pointer for allocate operation using MVD.
        DeallocateFn deallocateFn;       ///< Function pointer for deallocate operation using MVD.
        DeallocateAllFn deallocateAllFn; ///< Function pointer for deallocateAll operation using MVD.
    };

}