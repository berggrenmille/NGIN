#pragma once

namespace NGIN::TypeErasure
{
    class DynamicStoragePolicy
    {
    public:
        DynamicStoragePolicy() = delete;

        template <typename T>
        DynamicStoragePolicy(const T &obj)
        {
            ptr = new T(obj);
            destructor = [](void *obj)
            {
                delete static_cast<T *>(obj);
            };
        }

        template <typename T>
        DynamicStoragePolicy(T &&obj)
        {
            ptr = new T(std::move(obj));
            destructor = [](void *obj)
            {
                delete static_cast<T *>(obj);
            };
        }

        ~DynamicStoragePolicy()
        {
            destructor(ptr);
        }

        void *get() const { return ptr; }

    private:
        void *ptr = nullptr;
        void (*destructor)(void *) = nullptr;
    };
}