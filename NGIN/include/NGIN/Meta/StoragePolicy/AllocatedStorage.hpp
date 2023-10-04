#pragma once

#include <NGIN/Defines.hpp>
#include "Concepts.hpp"

namespace NGIN::Meta::StoragePolicy
{

    /// \class AllocatedStorage
    /// \brief A storage policy that allocates memory using an NGIN Allocator.
    /// This storage policy is useful for storing objects that are too large to fit on the stack.
    /// It is also useful for storing objects where the memory layout needs to be controlled
    class AllocatedStorage
    {
    public:
        AllocatedStorage() = default;

        AllocatedStorage(const AllocatedStorage &) = delete;

        AllocatedStorage &operator=(const AllocatedStorage &other) = delete;

        AllocatedStorage(AllocatedStorage &&other) noexcept;

        ~AllocatedStorage();

        template<IsStorageWrappable T, typename AllocatorType>
        AllocatedStorage(T &&obj, AllocatorType &alloc)
        requires IsNotSame<AllocatedStorage, T>;


        [[nodiscard]] inline void *get()
        { return ptr; }

        [[nodiscard]] inline void *get() const
        { return ptr; }


    private:
        void *ptr = nullptr;
        void *alloc = nullptr;

        void (*destructor)(void *, void *) = nullptr;
    };

    template<IsStorageWrappable T, typename AllocatorType>
    AllocatedStorage::AllocatedStorage(T &&obj, AllocatorType &alloc)
    requires IsNotSame<AllocatedStorage, T>
            : alloc(&alloc)
    {
        using StoredType = std::decay_t<T>;
        ptr = alloc.Allocate(sizeof(StoredType), alignof(StoredType));
        new(ptr) StoredType(std::forward<T>(obj));

        destructor = [](void *obj, void *alloc)
        {
            static_cast<StoredType *>(obj)->~StoredType();
            static_cast<AllocatorType *>(alloc)->Deallocate(obj);
        };
    }

    inline AllocatedStorage::AllocatedStorage(Meta::StoragePolicy::AllocatedStorage &&other) noexcept
            : ptr(other.ptr), destructor(other.destructor), alloc(other.alloc)
    {
        other.ptr = nullptr;
        other.destructor = nullptr;
        other.alloc = nullptr;
    }

    inline AllocatedStorage::~AllocatedStorage()
    {
        if (destructor)
            destructor(ptr, alloc);
    }

} // namespace NGIN::Util