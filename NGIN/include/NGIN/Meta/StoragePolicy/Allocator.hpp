#pragma once

#include <NGIN/Meta/StoragePolicy.hpp>
namespace NGIN::Meta::StoragePolicy
{
    template <typename AllocatorType>
    class Allocator
    {
    public:
        Allocator() = delete;

        template <typename T>
        Allocator(const T &obj, AllocatorType &alloc)
            : allocator(alloc)
        {
            using StoredType = std::decay_t<T>;
            ptr = alloc.New<StoredType>(obj);
            destructor = [&](void *obj)
            {
                alloc.Delete(static_cast<StoredType *>(obj));
            };
        }

        template <typename T>
        Allocator(T &&obj, AllocatorType &alloc)
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

        void *get() const { return ptr; }

    private:
        AllocatorType &allocator;
        void *ptr = nullptr;
        void (*destructor)(void *) = nullptr;
    };
} // namespace NGIN::Util