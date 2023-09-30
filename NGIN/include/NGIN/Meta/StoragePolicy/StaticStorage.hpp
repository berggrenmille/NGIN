#pragma once

#include <cstddef>
#include <cstring>
#include "Concepts.hpp"

#define NGIN_STATIC_STORAGE_ALIGNMENT 16
#define NGIN_STATIC_STORAGE_TO_STRING(x) #x
#if defined(_MSC_VER)
#define NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE __declspec(align(NGIN_STATIC_STORAGE_ALIGNMENT))
#else
#define NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE alignas(NGIN_STATIC_STORAGE_ALIGNMENT)
#endif

namespace NGIN::Meta::StoragePolicy
{

    template<std::size_t Size = 64>
    class NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE StaticStorage
    {

    public:
        /// @brief Default constructor.
        /// Initializes the internal storage.
        StaticStorage();

        /// @brief Copy constructor (deleted).
        /// StoragePolicies can only be moved.
        StaticStorage(const StaticStorage& other) = delete;

        /// @brief Move assignment operator.
        /// @param other The object to be moved.
        /// @return Reference to the moved object.
        StaticStorage& operator=(StaticStorage&& other) noexcept;

        /// @brief Move constructor.
        /// @param other The object to be moved.
        StaticStorage(StaticStorage&& other) noexcept;

        /// @brief Construct from a storage wrappable type.
        /// @tparam T Type of the object to be stored.
        /// @param obj The object to be stored.
        template<IsStorageWrappable T>
        StaticStorage(T&& obj)
        requires IsNotSame<StaticStorage<Size>, T>;

        /// @brief Destructor.
        /// Calls the destructor of the stored object.
        ~StaticStorage();

        /// @brief Get a pointer to the internal buffer.
        /// @return Pointer to the internal buffer.
        [[nodiscard]] inline void* get()
        { return &buffer[0]; }

        [[nodiscard]] inline void* get() const
        { return const_cast<void*>(&buffer[0]); }

    private:
        /// @brief Internal buffer to hold object data.
        std::byte buffer[Size];

        /// @brief Function pointer for object destruction.
        void (* destructorFunc)(void*) = nullptr;

        /// @brief Function pointer for moving object data.
        void (* moveFunc)(void* dst, void* src) = nullptr;
    };

    template<std::size_t Size>
    StaticStorage<Size>::StaticStorage()
            : destructorFunc(nullptr), buffer {}
    {
    }

    template<std::size_t Size>
    StaticStorage<Size>& StaticStorage<Size>::operator=(StaticStorage&& other) noexcept
    {
        destructorFunc = other.destructorFunc;
        moveFunc = other.moveFunc;
        if (moveFunc)
            moveFunc(get(), other.get());
        else
            std::memcpy(get(), other.get(), Size);

        other.destructorFunc = nullptr;
        other.moveFunc = nullptr;

        return *this;
    }

    template<std::size_t Size>
    StaticStorage<Size>::StaticStorage(StaticStorage&& other) noexcept
            : buffer {}
    {
        destructorFunc = other.destructorFunc;
        moveFunc = other.moveFunc;
        if (moveFunc)
            moveFunc(get(), other.get());
        else
            std::memcpy(get(), other.get(), Size);

        other.destructorFunc = nullptr;
        other.moveFunc = nullptr;
    }

    template<std::size_t Size>
    template<IsStorageWrappable T>
    StaticStorage<Size>::StaticStorage(T&& obj)
    requires IsNotSame<StaticStorage<Size>, T>
    {
        using StoredType = std::decay_t<T>;
        static_assert(sizeof(StoredType) <= Size, "Type too large for StaticStorage.");
        static_assert(alignof(StoredType) <= NGIN_STATIC_STORAGE_ALIGNMENT,
                      "Type alignment too large for StaticStorage.");

        new(&buffer[0]) StoredType(std::move(obj));
        if constexpr (std::is_destructible_v<StoredType>)
        {
            destructorFunc = [](void* ptr)
            {
                static_cast<StoredType*>(ptr)->~StoredType();
            };
        }

        if constexpr (std::is_move_assignable_v<StoredType>)
        {
            moveFunc = [](void* dst, void* src)
            {
                *static_cast<StoredType*>(dst) = std::move(*static_cast<StoredType*>(src));
            };
        }
    }

    template<std::size_t Size>
    StaticStorage<Size>::~StaticStorage()
    {
        if (destructorFunc)
            destructorFunc(&buffer[0]);
    }

}