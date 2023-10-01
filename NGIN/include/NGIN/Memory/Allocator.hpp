
#pragma once
#include <NGIN/Defines.hpp>
#include "Concepts.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>
#include <concepts>
#include <source_location>
#include <memory>

namespace NGIN::Memory
{

    /// \class Allocator
    /// \brief Represents a generic memory allocator with dynamic dispatch.
    /// \tparam StoragePolicy The storage policy for wrapped allocator.
    ///
    /// The Allocator class manages objects with the expected Allocate, Deallocate, and DeallocateAll interface.
    /// Instead of using virtual functions, it employs manual vtable pointers for dynamic dispatch.
    template <typename StoragePolicy>
    class Allocator
    {
    public:
        Allocator()
            : pImpl{}
        {
        }
        Allocator(const Allocator &) = delete;
        Allocator(Allocator &&other) noexcept
            : pImpl(std::move(other.pImpl)),
              allocateFn(other.allocateFn),
              deallocateFn(other.deallocateFn),
              deallocateAllFn(other.deallocateAllFn),
              ownsFn(other.ownsFn)
        {

            other.allocateFn = nullptr;
            other.deallocateFn = nullptr;
            other.deallocateAllFn = nullptr;
            other.ownsFn = nullptr;
        }

        Allocator &operator=(Allocator &&other) noexcept
        {
            pImpl = std::move(other.pImpl);
            allocateFn = other.allocateFn;
            deallocateFn = other.deallocateFn;
            deallocateAllFn = other.deallocateAllFn;
            ownsFn = other.ownsFn;

            other.allocateFn = nullptr;
            other.deallocateFn = nullptr;
            other.deallocateAllFn = nullptr;
            other.ownsFn = nullptr;

            return *this;
        }
        /// \brief Constructs a new Allocator object.
        /// \tparam T The type of the allocator object.
        /// \param alloc The allocator object.
        template <IsAllocator T>
        Allocator(T &&alloc)
            requires IsNotSame<Allocator, T>
            : pImpl(std::forward<T>(alloc))
        {
            SetupFunctionPointers<T>();
        }

        /// \brief Allocates memory with specified size and alignment.
        /// \param size Size of memory to allocate.
        /// \param alignment Alignment for the memory (defaults to max alignment).
        /// \param location Source location of the allocation request (defaults to current location).
        /// \return Pointer to allocated memory, or nullptr if allocation fails.
        void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t), const std::source_location &location = std::source_location::current())
        {
            return allocateFn(pImpl.get(), size, alignment, location);
        }

        /// \brief Deallocates the provided memory.
        /// \param ptr Pointer to the memory segment to deallocate
        void Deallocate(void *ptr)
        {

            deallocateFn(pImpl.get(), ptr);
        }

        /// \brief Deallocates all managed memory by the allocator.
        void DeallocateAll()
        {

            deallocateAllFn(pImpl.get());
        }

        /// \brief Checks if the allocator owns the provided memory.
        /// \param ptr Pointer to the memory segment to check.
        /// \return True if the memory is owned, otherwise false.
        bool Owns(void *ptr)
        {
            return ownsFn(pImpl.get(), ptr);
        }

        /// \brief Allocates memory and constructs an object of type T.
        ///
        /// This method reserves memory for an object of type T and constructs the object in-place.
        /// \tparam T The object type to allocate and construct.
        /// \tparam Args Types of the constructor arguments.
        /// \param loc Source location for debugging (defaults to current location).
        /// \param args Arguments forwarded to T's constructor.
        /// \return Pointer to the constructed object or nullptr if allocation fails.
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

        /// \brief Calls the destructor for the object and deallocates its memory.
        ///
        /// Calls the destructor of the object of type T and deallocates its memory using the allocator.
        /// \tparam T The type of the object to destroy and deallocate.
        /// \param obj Pointer to the object.
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
        StoragePolicy pImpl;

        // Type definitions for function pointers.
        using AllocateFn = void *(*)(void *, size_t, size_t, const std::source_location &);
        using DeallocateFn = void (*)(void *, void *);
        using DeallocateAllFn = void (*)(void *);
        using OwnsFn = bool (*)(void *, void *);

        AllocateFn allocateFn = nullptr;
        DeallocateFn deallocateFn = nullptr;
        DeallocateAllFn deallocateAllFn = nullptr;
        OwnsFn ownsFn = nullptr;

        /// \brief Setups function pointers based on the allocator type.
        /// \tparam T Allocator type.
        template <typename T>
        void SetupFunctionPointers()
        {
            allocateFn = [](void *obj, size_t size, size_t alignment, const std::source_location &location) -> void *
            {
                if constexpr (HasAllocate<T>)
                {
                    return reinterpret_cast<T *>(obj)->Allocate(size, alignment, location);
                }
                return nullptr;
            };

            deallocateFn = [](void *obj, void *ptr)
            {
                if constexpr (HasDeallocate<T>)
                {
                    reinterpret_cast<T *>(obj)->Deallocate(ptr);
                }
            };

            deallocateAllFn = [](void *obj)
            {
                if constexpr (HasDeallocateAll<T>)
                {
                    reinterpret_cast<T *>(obj)->DeallocateAll();
                }
            };

            ownsFn = [](void *obj, void *ptr)
            {
                if constexpr (HasOwns<T>)
                {
                    return reinterpret_cast<T *>(obj)->Owns(ptr);
                }
                return false;
            };
        }
    };

} // namespace NGIN::Memory
