#pragma once
#include <cstddef>
#include <memory>

namespace NGIN::TypeErasure
{
    /// \class HybridStoragePolicy
    /// \tparam Size The size of the static buffer, defaults to 128 bytes.
    /// \brief A class for dynamic and static storage management.
    ///
    /// The HybridStoragePolicy class is designed to handle storage for objects either on the stack or heap.
    /// If the object fits within the buffer size specified by Size, it is stored statically.
    /// Otherwise, it is stored dynamically on the heap.
    template <std::size_t Size = 128>
    class
#ifdef _MSC_VER
        __declspec(align(16))
#else
        alignas(16)
#endif
            HybridStoragePolicy
    {
        static_assert(Size % 16 == 0, "Size must be a multiple of 16.");

    public:
        /// \brief Default constructor is deleted to prevent empty initialization.
        HybridStoragePolicy() = delete;

        /// \brief Construct HybridStoragePolicy with an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be stored.
        template <typename T>
        HybridStoragePolicy(const T &obj)
        {
            if (sizeof(T) <= Size)
            {
                // Using placement new here
                new (&data.buffer[0]) T(obj);
                SetDestructor([](void *obj)
                              { reinterpret_cast<T *>(obj)->~T(); },
                              false);
            }
            else
            {
                data.ptr = new T(obj);
                SetDestructor([](void *obj)
                              { delete static_cast<T *>(obj); },
                              true);
            }
        }

        /// \brief Construct HybridStoragePolicy by moving an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be moved into storage.
        template <typename T>
        HybridStoragePolicy(T &&obj)
        {
            if (sizeof(T) <= Size)
            {
                // Using placement new here
                new (&data.buffer[0]) T(std::move(obj));
                SetDestructor([](void *obj)
                              { reinterpret_cast<T *>(obj)->~T(); },
                              false);
            }
            else
            {
                data.ptr = new T(std::move(obj));
                SetDestructor([](void *obj)
                              { delete static_cast<T *>(obj); },
                              true);
            }
        }

        /// \brief Destructor for HybridStoragePolicy.
        ///
        /// Calls the destructor function pointer to clean up the stored object, whether it's on the stack or heap.
        ~HybridStoragePolicy()
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
