#pragma once
#include <NGIN/Memory/Allocator.hpp>
namespace NGIN::Util
{
    class AllocatorStoragePolicy
    {
    public:
        AllocatorStoragePolicy() = delete;

        template <typename T>
        AllocatorStoragePolicy(const T &obj, NGIN::Memory::Allocator &alloc)
            : allocator(alloc)
        {
            ptr = alloc.New<T>(obj);
            destructor = [&](void *obj)
            {
                alloc.Delete(static_cast<T *>(obj));
            };
        }

        template <typename T>
        AllocatorStoragePolicy(T &&obj, Memory::Allocator &alloc)
            : allocator(alloc)
        {
            ptr = alloc.New<T>(std::move(obj));

            destructor = [&](void *obj)
            {
                alloc.Delete(static_cast<T *>(obj));
            };
        }

        ~AllocatorStoragePolicy()
        {
            destructor(ptr);
        }

        void *get() const { return ptr; }

    private:
        Memory::Allocator &allocator;
        void *ptr = nullptr;
        void (*destructor)(void *) = nullptr;
    };
} // namespace NGIN::Util