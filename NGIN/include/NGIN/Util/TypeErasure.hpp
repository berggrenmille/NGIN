#pragma once
#include <new>
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

        void *get() const { return reinterpret_cast<void *>(buffer); }

    private:
        std::aligned_storage_t<Size, alignof(std::max_align_t)> buffer;
        void (*destructor)(void *) = nullptr;
    };

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
                new (buffer) T(std::move(obj));
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
                destructor(ptr ? ptr : buffer);
            }
        }

        void *get() const
        {
            return ptr ? ptr : reinterpret_cast<void *>(buffer);
        }

    private:
        void *ptr = nullptr;
        std::aligned_storage_t<Size, alignof(std::max_align_t)> buffer;
        void (*destructor)(void *) = nullptr;
    };
}