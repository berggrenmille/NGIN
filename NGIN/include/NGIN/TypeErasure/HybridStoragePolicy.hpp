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
    class HybridStoragePolicy
    {
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

        /// \brief Construct HybridStoragePolicy by moving an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be moved into storage.
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

        /// \brief Destructor for HybridStoragePolicy.
        ///
        /// Calls the destructor function pointer to clean up the stored object, whether it's on the stack or heap.
        ~HybridStoragePolicy()
        {
            if (destructor)
            {
                destructor(ptr ? ptr : &buffer[0]);
            }
        }

        /// \brief Get the stored object as a void pointer.
        /// \return A void pointer to the stored object.
        void *get()
        {
            return ptr ? ptr : &buffer[0];
        }

    private:
        /// \brief A pointer to the dynamically stored object, if used.
        void *ptr = nullptr;

        /// \brief A buffer for static storage.
        ///
        /// Aligning to max_align_t to ensure that the buffer is correctly aligned for any type T.
        alignas(alignof(std::max_align_t)) std::byte buffer[Size];

        /// \brief A function pointer for the destructor of the stored object.
        ///
        /// This function pointer will point to the appropriate destructor based on the type of object stored.
        /// It is used in the destructor to clean up the object.
        void (*destructor)(void *) = nullptr;
    };
}
