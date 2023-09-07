#pragma once
#include <cstddef>

namespace NGIN::Meta
{

    template <std::size_t Size = 64>
    class
#ifdef _MSC_VER
        __declspec(align(16))
#else
        alignas(16)
#endif
            StaticStoragePolicy
    {
        static_assert(Size % 16 == 0, "Size must be a multiple of 16.");

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
        void (*destructor)(void *) = nullptr;

        std::byte buffer[Size];
    };
}