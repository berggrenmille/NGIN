#pragma once

namespace NGIN::Meta
{
    /// \class DynamicStoragePolicy
    /// \brief A class for dynamic storage management.
    ///
    /// The DynamicStoragePolicy class is a utility for dynamically managing storage.
    /// It supports dynamic allocation and deallocation of objects of different types.
    /// This is particularly useful for type erasure techniques where the type of the object
    /// is required to be decoupled from the interface.
    class DynamicStoragePolicy
    {
    public:
        /// \brief Default constructor is deleted to prevent empty initialization.
        DynamicStoragePolicy() = delete;

        /// \brief Construct DynamicStoragePolicy with an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be stored.
        template <typename T>
        DynamicStoragePolicy(const T &obj)
        {
            ptr = new T(obj);
            destructor = [](void *obj)
            {
                delete static_cast<T *>(obj);
            };
        }

        /// \brief Construct DynamicStoragePolicy by moving an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be moved into storage.
        template <typename T>
        DynamicStoragePolicy(T &&obj)
        {
            ptr = new T(std::move(obj));
            destructor = [](void *obj)
            {
                delete static_cast<T *>(obj);
            };
        }

        /// \brief Destructor for DynamicStoragePolicy.
        ///
        /// Calls the destructor function pointer to clean up the dynamically allocated object.
        ~DynamicStoragePolicy()
        {
            destructor(ptr);
        }

        /// \brief Get the stored object as a void pointer.
        /// \return A void pointer to the stored object.
        void *get() const { return ptr; }

    private:
        /// \brief A pointer to the stored object.
        void *ptr = nullptr;

        /// \brief A function pointer for the destructor of the stored object.
        ///
        /// This function pointer will point to the appropriate destructor based on
        /// the type of object stored. It is used in the destructor to clean up the object.
        void (*destructor)(void *) = nullptr;
    };
}
