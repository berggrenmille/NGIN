#pragma once

#include "Concepts.hpp"

namespace NGIN::Meta::StoragePolicy
{
    /// \class DynamicStorage
    /// \brief A class for dynamic storage management.
    ///
    /// The DynamicStorage class is a utility for dynamically managing storage.
    /// It supports dynamic allocation and deallocation of objects of different types.
    /// This is particularly useful for type erasure techniques where the type of the object
    /// is required to be decoupled from the interface.
    class DynamicStorage
    {
    public:
        /// \brief Default constructor is deleted to prevent empty initialization.
        DynamicStorage() = default;

        DynamicStorage(const DynamicStorage &) = delete;

        DynamicStorage &operator=(const DynamicStorage &other) = delete;

        DynamicStorage(DynamicStorage &&other) noexcept;

        DynamicStorage &operator=(DynamicStorage &&other) noexcept;

        /// \brief Construct DynamicStorage by moving an object.
        /// \tparam T The type of the object.
        /// \param obj The object to be moved into storage.
        template<IsStorageWrappable T>
        DynamicStorage(T &&obj)
        requires IsNotSame<DynamicStorage, T>;

        /// \brief Destructor for DynamicStorage.
        ///
        /// Calls the destructor function pointer to clean up the dynamically allocated object.
        ~DynamicStorage();

        /// \brief Get the stored object as a void pointer.
        /// \return A void pointer to the stored object.
        void *get();

    private:
        /// \brief A pointer to the stored object.
        void *ptr = nullptr;

        /// \brief A function pointer for the destructor of the stored object.
        ///
        /// This function pointer will point to the appropriate destructor based on
        /// the type of object stored. It is used in the destructor to clean up the object.
        void (*destructor)(void *) = nullptr;
    };

    inline DynamicStorage::DynamicStorage(DynamicStorage &&other) noexcept
    {
        if (destructor)
            destructor(ptr);

        ptr = other.ptr;
        destructor = other.destructor;

        other.ptr = nullptr;
        other.destructor = nullptr;
    }

    inline DynamicStorage &DynamicStorage::operator=(DynamicStorage &&other) noexcept
    {
        if (destructor)
            destructor(ptr);

        ptr = other.ptr;
        destructor = other.destructor;

        other.ptr = nullptr;
        other.destructor = nullptr;

        return *this;
    }

    template<IsStorageWrappable T>
    DynamicStorage::DynamicStorage(T &&obj)
    requires IsNotSame<DynamicStorage, T> // NOLINT(*-forwarding-reference-overload)
    {
        using StoredType = std::decay_t<T>;
        ptr = new StoredType(std::forward<T>(obj));
        destructor = [](void *objPtr)
        {
            delete static_cast<StoredType *>(objPtr);
        };
    }

    inline DynamicStorage::~DynamicStorage()
    {
        if (destructor)
            destructor(ptr);
    }

    inline void *DynamicStorage::get()
    {
        return ptr;
    }
}
