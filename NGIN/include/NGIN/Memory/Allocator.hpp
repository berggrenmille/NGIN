#pragma once
#include <NGIN/Core.h>
#include <NGIN/Util/TypeErasure.hpp>

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
     */
    template <typename T>
    concept AllocatorConcept = requires(T alloc, size_t size, size_t alignment, const std::source_location &location) {
        alloc.Allocate(size, alignment, location);
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
            : pimpl(new Model<AllocatorT>(std::move(alloc)), NGIN::Util::Deleter(pimpl.get()))

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

        /**
         * @brief Allocates memory for an object of type T from the provided allocator and constructs it.
         *
         * Uses the allocator to reserve memory for an object of type T, and then constructs the object
         * in-place using the provided arguments.
         *
         * @tparam T The type of the object to allocate and construct.
         * @tparam Alloc The type of the allocator. Should satisfy the AllocatorConcept.
         * @tparam Args Variadic template for the arguments required to construct the object of type T.
         * @param allocator Reference to the allocator instance.
         * @param loc Source location information for potential debugging purposes.
         * @param args Arguments forwarded to the constructor of T.
         * @return Pointer to the constructed object, or nullptr if allocation fails.
         */
        template <typename T, typename... Args>
        T *New(const std::source_location &loc = std::source_location::current(), Args &&...args)
        {
            void *memory = Allocate(sizeof(T), alignof(T), loc);
            if (!memory)
            {
                // Handle allocation failure.
                return nullptr;
            }
            return new (memory) T(std::forward<Args>(args)...);
        }

        /**
         * @brief Deallocates an object of type T using the provided allocator and calls its destructor.
         *
         * Calls the destructor of the object pointed to by the provided pointer and then
         * deallocates its memory using the allocator.
         *
         * @tparam T The type of the object to destroy and deallocate.
         * @tparam Alloc The type of the allocator. Should satisfy the AllocatorConcept.
         * @param allocator Reference to the allocator instance.
         * @param obj Pointer to the object to destroy and deallocate.
         */
        template <typename T>
        void Delete(T *obj)
        {
            if (obj) [[likely]]
            {
                obj->~T();
                Deallocate(obj);
            }
        }

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
                : allocator(std::move(allocator)) {}

            void *Allocate(size_t size, size_t alignment, const std::source_location &location)
            {
                return allocator.Allocate(size, alignment, location);
            }

            void Deallocate(void *ptr)
            {
                allocator.Deallocate(ptr);
            }

            void DeallocateAll()
            {
                allocator.DeallocateAll();
            }

        private:
            AllocatorT allocator; ///< Underlying instance of the wrapped allocator.
        };

        std::unique_ptr<void, NGIN::Util::Deleter> pimpl; ///< Underlying instance of the wrapped allocator.

        using AllocateFn = void *(*)(void *, size_t, size_t, const std::source_location &);
        using DeallocateFn = void (*)(void *, void *);
        using DeallocateAllFn = void (*)(void *);
        AllocateFn allocateFn;           ///< Function pointer for allocate operation using MVD.
        DeallocateFn deallocateFn;       ///< Function pointer for deallocate operation using MVD.
        DeallocateAllFn deallocateAllFn; ///< Function pointer for deallocateAll operation using MVD.
    };
}
