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
            : allocator(alloc), ptr(alloc.New<T>(obj))
        {

            destructor = [&](void *obj)
            {
                alloc.Delete(static_cast<T *>(obj));
            };
        }

        template <typename T>
        Allocator(T &&obj, AllocatorType &alloc)
            : allocator(alloc)
        {
            ptr = alloc.New<T>(std::move(obj));

            destructor = [&](void *obj)
            {
                alloc.Delete(static_cast<T *>(obj));
            };
        }

        ~Allocator()
        {
            destructor(ptr);
        }

        void *get() const { return ptr; }

    private:
        AllocatorType &allocator;
        void *ptr = nullptr;
        void (*destructor)(void *) = nullptr;
    };
} // namespace NGIN::Util