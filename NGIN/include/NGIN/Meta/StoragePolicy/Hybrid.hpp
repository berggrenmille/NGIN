#pragma once
#include <cstddef>
#include <memory>
#include <cstring>

#define NGIN_HYBRID_STORAGE_ALIGNMENT 16
#define NGIN_HYBRID_STORAGE_TO_STRING(x) #x
#if defined(_MSC_VER)
#define NGIN_HYBRID_STORAGE_ALIGNMENT_ATTRIBUTE __declspec(align(NGIN_HYBRID_STORAGE_ALIGNMENT))
#else
#define NGIN_HYBRID_STORAGE_ALIGNMENT_ATTRIBUTE alignas(NGIN_HYBRID_STORAGE_ALIGNMENT)
#endif

namespace NGIN::Meta::StoragePolicy
{
    /// \class Hybrid
    /// \tparam Size The size of the static buffer, defaults to 128 bytes.
    /// \brief A class for dynamic and static storage management.
    ///
    /// The Hybrid class is designed to handle storage for objects either on the stack or heap.
    /// If the object fits within the buffer size specified by Size, it is stored statically.
    /// Otherwise, it is stored dynamically on the heap.
    template <std::size_t Size = 128>
    class NGIN_HYBRID_STORAGE_ALIGNMENT_ATTRIBUTE Hybrid
    {
        static_assert(Size % NGIN_HYBRID_STORAGE_ALIGNMENT == 0, "Size must be a multiple of " NGIN_HYBRID_STORAGE_TO_STRING(NGIN_HYBRID_STORAGE_ALIGNMENT) ".");

    public:
        /// \brief Default constructor is deleted to prevent empty initialization.
        Hybrid() = delete;

        /// \brief Construct Hybrid with an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be stored.
        template <typename T>
        Hybrid(const T &obj)
        {
            using StoredType = std::decay_t<T>;
            if constexpr (std::is_same<StoredType, Hybrid<Size>>::value)
            {
                /// TODO: MIGHT BE BROKEN. CHECK THIS.
                std::memcpy(&data.buffer[0], &obj.data.buffer[0], sizeof(StoredType));
                destructor = obj.destructor;
            }
            else
            {
                if constexpr (sizeof(T) <= Size)
                {
                    // Using placement new here
                    new (&data.buffer[0]) StoredType(obj);
                    SetDestructor([](void *obj)
                                  { reinterpret_cast<StoredType *>(obj)->~T(); },
                                  false);
                }
                else
                {
                    data.ptr = new StoredType(obj);
                    SetDestructor([](void *obj)
                                  { delete static_cast<StoredType *>(obj); },
                                  true);
                }
            }
        }

        /// \brief Construct Hybrid by moving an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be moved into storage.
        template <typename T>
        Hybrid(T &&obj)
        {
            using StoredType = std::decay_t<T>;
            if constexpr (std::is_same<StoredType, Hybrid<Size>>::value)
            {
                std::memcpy(&data.buffer[0], &obj.data.buffer[0], sizeof(StoredType));
                destructor = obj.destructor;

                obj.data.ptr = nullptr;
                obj.destructor = nullptr;
            }
            else
            {
                if constexpr (sizeof(T) <= Size)
                {
                    // Using placement new here
                    new (&data.buffer[0]) StoredType(std::move(obj));
                    SetDestructor([](void *obj)
                                  { reinterpret_cast<StoredType *>(obj)->~T(); },
                                  false);
                }
                else
                {
                    data.ptr = new StoredType(std::move(obj));
                    SetDestructor([](void *obj)
                                  { delete static_cast<StoredType *>(obj); },
                                  true);
                }
            }
        }

        /// \brief Destructor for Hybrid.
        ///
        /// Calls the destructor function pointer to clean up the stored object, whether it's on the stack or heap.
        ~Hybrid()
        {
            if (destructor)
            {
                void (*actualDestructor)(void *) = reinterpret_cast<void (*)(void *)>(reinterpret_cast<std::uintptr_t>(destructor) & ~std::uintptr_t(1));
                actualDestructor(UseHeap() ? data.ptr : &data.buffer[0]);
            }
        }

        /// \brief Get the stored object as a void pointer.
        /// \return A void pointer to the stored object.
        void *get()
        {
            return UseHeap() ? data.ptr : &data.buffer[0];
        }

    private:
        /// \brief A function pointer for the destructor of the stored object.
        ///
        /// This function pointer will point to the appropriate destructor based on the type of object stored.
        /// It is used in the destructor to clean up the object.
        void (*destructor)(void *) = nullptr;

        union Data
        {
            void *ptr;
            std::byte buffer[Size];
        } data;

        void SetDestructor(void (*d)(void *), bool heap)
        {
            destructor = reinterpret_cast<void (*)(void *)>((reinterpret_cast<uintptr_t>(d) & ~uintptr_t(1)) | static_cast<uintptr_t>(heap));
        }

        inline bool UseHeap() const
        {
            return reinterpret_cast<uintptr_t>(destructor) & 1;
        }
    };
}
