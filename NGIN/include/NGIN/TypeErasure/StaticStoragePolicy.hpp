#pragma once
#include <cstddef>

namespace NGIN::TypeErasure
{

    template <std::size_t Size = 128>
    class StaticStoragePolicy
    {
    public:
        StaticStoragePolicy() = delete;

        template <typename T>
        StaticStoragePolicy(const T &obj)
        {
            static_assert(sizeof(T) <= Size, "Type too large for StaticStoragePolicy.");
            new (&buffer[0]) T(obj);
            destructor = [](void *obj)
            {
                reinterpret_cast<T *>(obj)->~T();
            };
        }

        template <typename T>
        StaticStoragePolicy(T &&obj)
        {
            static_assert(sizeof(T) <= Size, "Type too large for StaticStoragePolicy.");
            new (&buffer[0]) T(std::move(obj));
            destructor = [](void *obj)
            {
                reinterpret_cast<T *>(obj)->~T();
            };
        }

        ~StaticStoragePolicy()
        {
            if (destructor)
            {
                destructor(&buffer[0]);
            }
        }

        void *get() { return &buffer[0]; }

    private:
        alignas(alignof(std::max_align_t)) std::byte buffer[Size];

        void (*destructor)(void *) = nullptr;
    };
}