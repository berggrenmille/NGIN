#pragma once
#include <cstddef>
#include <cstring>

#define NGIN_STATIC_STORAGE_ALIGNMENT 16
#define NGIN_STATIC_STORAGE_TO_STRING(x) #x
#if defined(_MSC_VER)
#define NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE __declspec(align(NGIN_STATIC_STORAGE_ALIGNMENT))
#else
#define NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE alignas(NGIN_STATIC_STORAGE_ALIGNMENT)
#endif

namespace NGIN::Meta::StoragePolicy
{

    template <std::size_t Size = 64>
    class NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE Static
    {
        static_assert(Size % NGIN_STATIC_STORAGE_ALIGNMENT == 0, "Size must be a multiple of " NGIN_STATIC_STORAGE_TO_STRING(NGIN_STATIC_STORAGE_ALIGNMENT) ".");

    public:
        Static() = delete;

        template <typename T>
        Static(const T &obj)
        {
            using StoredType = std::decay_t<T>;
            if constexpr (std::is_same<StoredType, Static<Size>>::value)
            {
                /// TODO: Might be broken. Check this.
                std::memcpy(&buffer[0], &obj.buffer[0], sizeof(StoredType));
                destructor = obj.destructor;
            }
            else
            {
                new (&buffer[0]) StoredType(obj);
                destructor = [](void *obj)
                {
                    reinterpret_cast<StoredType *>(obj)->~T();
                };
            }
        }

        template <typename T>
        Static(T &&obj)
        {
            static_assert(sizeof(T) <= Size, "Type too large for Static.");
            using StoredType = std::decay_t<T>;

            new (&buffer[0]) StoredType(std::move(obj));
            destructor = [](void *obj)
            {
                reinterpret_cast<StoredType *>(obj)->~T();
            };
        }

        ~Static()
        {
            if (destructor)

                destructor(&buffer[0]);
        }

        void *get() { return &buffer[0]; }

    private:
        void (*destructor)(void *) = nullptr;

        std::byte buffer[Size];
    };
}