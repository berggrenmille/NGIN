#pragma once
#include "Concepts.hpp"
namespace NGIN::Meta::StoragePolicy
{

    class AllocatedStorage
    {
    public:
        AllocatedStorage() = delete;

        AllocatedStorage(const AllocatedStorage &) = delete;

        template <IsStorageWrappable T, typename AllocatorType>
        AllocatedStorage(T &&obj, AllocatorType &alloc)
            requires IsNotSame<AllocatedStorage, T>
            : alloc(&alloc)
        {
            using StoredType = std::decay_t<T>;
            ptr = alloc.Allocate(sizeof(StoredType), alignof(StoredType));
            new (ptr) StoredType(std::move(obj));

            destructor = [](void *obj, void *alloc)
            {
                static_cast<StoredType *>(obj)->~StoredType();
                static_cast<AllocatorType *>(alloc)->Deallocate(obj);
            };
        }

        AllocatedStorage(AllocatedStorage &&other)
            : ptr(other.ptr), destructor(other.destructor), alloc(other.alloc)
        {
            other.ptr = nullptr;
            other.destructor = nullptr;
            other.alloc = nullptr;
        }

        ~AllocatedStorage()
        {
            if (destructor)
                destructor(ptr, alloc);
        }

        void *get() { return ptr; }

    private:
        void *ptr = nullptr;
        void *alloc = nullptr;
        void (*destructor)(void *, void *) = nullptr;
    };
} // namespace NGIN::Util