#pragma once

namespace NGIN::Util
{
    struct Deleter
    {
        void (*deleterFn)(void *) = nullptr;

        template <typename T>
        Deleter(T *)
        {
            deleterFn = [](void *ptr)
            {
                delete static_cast<T *>(ptr);
            };
        }

        void operator()(void *ptr) const
        {
            if (deleterFn)
            {
                deleterFn(ptr);
            }
        }
    };

    // DynamicStoragePolicy
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

    // StaticStoragePolicy
    template <std::size_t Size = 128>
    class StaticStoragePolicy
    {
    public:
        StaticStoragePolicy() = delete;

        template <typename T>
        StaticStoragePolicy(const T &obj)
        {
            static_assert(sizeof(T) <= Size, "Type too large for StaticStoragePolicy.");
            new (buffer) T(obj);
            destructor = [](void *obj)
            {
                reinterpret_cast<T *>(obj)->~T();
            };
        }

        template <typename T>
        StaticStoragePolicy(T &&obj)
        {
            static_assert(sizeof(T) <= Size, "Type too large for StaticStoragePolicy.");
            new (buffer) T(std::move(obj));
            destructor = [](void *obj)
            {
                reinterpret_cast<T *>(obj)->~T();
            };
        }

        ~StaticStoragePolicy()
        {
            if (destructor)
            {
                destructor(buffer);
            }
        }

        void *get() const { return const_cast<void *>(static_cast<const void *>(buffer)); }

    private:
        std::aligned_storage_t<Size, alignof(std::max_align_t)> buffer;
        void (*destructor)(void *) = nullptr;
    };

}