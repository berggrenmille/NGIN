#pragma once
#include "Concepts.hpp"
namespace NGIN::Meta::StoragePolicy
{
    template <typename AllocatorType>
    class Allocator
    {
    public:
        Allocator() = delete;

        Allocator(const Allocator &) = delete;

        template <IsStorageWrappable T>
        Allocator(T &&obj, AllocatorType &alloc)
            requires IsNotSame<Allocator, T>
            : allocator(alloc)
        {
            using StoredType = std::decay_t<T>;
            ptr = alloc.New<StoredType>(std::move(obj));

            destructor = [&](void *obj)
            {
                alloc.Delete(static_cast<StoredType *>(obj));
            };
        }

        Allocator(Allocator &&other)
            : allocator(other.allocator), ptr(other.ptr), destructor(other.destructor)
        {
            other.ptr = nullptr;
            other.destructor = nullptr;
        }

        ~Allocator()
        {
            if (destructor)
                destructor(ptr);
        }

        void *get() { return ptr; }

    private:
        AllocatorType &allocator;
        void *ptr = nullptr;
        void (*destructor)(void *) = nullptr;
    };
} // namespace NGIN::Util