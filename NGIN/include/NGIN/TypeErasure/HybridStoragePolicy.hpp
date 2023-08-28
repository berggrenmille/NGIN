#pragma once
#include <cstddef>
#include <memory>
namespace NGIN::TypeErasure
{

    template <std::size_t Size = 128>
    class HybridStoragePolicy
    {
    public:
        HybridStoragePolicy() = delete;

        template <typename T>
        HybridStoragePolicy(const T &obj)
        {
            if (sizeof(T) <= Size)
            {
                // Using placement new here
                new (buffer) T(obj);
                destructor = [](void *obj)
                {
                    reinterpret_cast<T *>(obj)->~T();
                };
                ptr = nullptr; // Indicate static storage is used
            }
            else
            {
                ptr = new T(obj);
                destructor = [](void *obj)
                {
                    delete static_cast<T *>(obj);
                };
            }
        }

        template <typename T>
        HybridStoragePolicy(T &&obj)
        {
            if (sizeof(T) <= Size)
            {
                // Using placement new here
                new (&buffer[0]) T(std::move(obj));
                destructor = [](void *obj)
                {
                    reinterpret_cast<T *>(obj)->~T();
                };
                ptr = nullptr; // Indicate static storage is used
            }
            else
            {
                ptr = new T(std::move(obj));

                destructor = [](void *obj)
                {
                    delete static_cast<T *>(obj);
                };
            }
        }

        ~HybridStoragePolicy()
        {
            if (destructor)
            {
                destructor(ptr ? ptr : &buffer[0]);
            }
        }

        void *get()
        {

            return ptr ? ptr : &buffer[0];
        }

    private:
        void *ptr = nullptr;
        alignas(alignof(std::max_align_t)) std::byte buffer[Size];
        void (*destructor)(void *) = nullptr;
    };
}