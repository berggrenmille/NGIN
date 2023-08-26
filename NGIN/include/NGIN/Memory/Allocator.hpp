
#pragma once
#include <NGIN/Core.h>
#include <NGIN/Util/TypeErasure.hpp>

#include "Internal/AllocatorConcepts.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>
#include <concepts>
#include <source_location>
#include <memory>

namespace NGIN::Memory
{

    /**
     * @class Allocator
     * @brief Represents a generic memory allocator.
     *
     * This class can manage any object that matches the expected interface (Allocate, Deallocate, DeallocateAll).
     * Instead of relying on virtual functions, it utilizes manual vtable pointers for a dynamic dispatch mechanism.
     */
    template <typename StoragePolicy = Util::DynamicStoragePolicy>
    class Allocator
    {
    public:
        Allocator() = delete;
        Allocator(const Allocator &) = delete;

        /**
         * @brief Construct a new Allocator object.
         *
         * @tparam T Type of the allocator object.
         * @param alloc The allocator object.
         */
        template <Internal::IsAllocator T>
        Allocator(T &&alloc)
            : pimpl(std::move(alloc))
        {
            SetupFunctionPointers<T>();
        }

        /**
         * @brief Allocate memory with given size and alignment.
         *
         * @param size The size of the memory to allocate.
         * @param alignment The alignment of the memory.
         * @param location The source location from where the allocation is requested.
         * @return void* Pointer to the allocated memory, or nullptr if allocation fails.
         */
        void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t), const std::source_location &location = std::source_location::current()) const
        {
            return allocateFn(pimpl.get(), size, alignment, location);
        }

        /**
         * @brief Deallocate the given memory.
         *
         * @param ptr Pointer to the memory to deallocate.
         */
        void Deallocate(void *ptr) const
        {

            deallocateFn(pimpl.get(), ptr);
        }

        /**
         * @brief Deallocate all managed memory.
         */
        void DeallocateAll() const
        {

            deallocateAllFn(pimpl.get());
        }

        bool Owns(void *ptr) const
        {
            return ownsFn(pimpl.get(), ptr);
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
        StoragePolicy pimpl;

        using AllocateFn = void *(*)(void *, size_t, size_t, const std::source_location &);
        using DeallocateFn = void (*)(void *, void *);
        using DeallocateAllFn = void (*)(void *);
        using OwnsFn = bool (*)(void *, void *);

        AllocateFn allocateFn = nullptr;
        DeallocateFn deallocateFn = nullptr;
        DeallocateAllFn deallocateAllFn = nullptr;
        OwnsFn ownsFn = nullptr;

        template <typename T>
        void SetupFunctionPointers()
        {
            allocateFn = [](void *obj, size_t size, size_t alignment, const std::source_location &location) -> void *
            {
                if constexpr (Internal::HasAllocate<T>)
                {
                    return reinterpret_cast<T *>(obj)->Allocate(size, alignment, location);
                }
                return nullptr;
            };

            deallocateFn = [](void *obj, void *ptr)
            {
                if constexpr (Internal::HasDeallocate<T>)
                {
                    reinterpret_cast<T *>(obj)->Deallocate(ptr);
                }
            };

            deallocateAllFn = [](void *obj)
            {
                if constexpr (Internal::HasDeallocateAll<T>)
                {
                    reinterpret_cast<T *>(obj)->DeallocateAll();
                }
            };

            ownsFn = [](void *obj, void *ptr)
            {
                if constexpr (Internal::HasOwns<T>)
                {
                    return reinterpret_cast<T *>(obj)->Owns(ptr);
                }
                return false;
            };
        }
    };

} // namespace NGIN::Memory
