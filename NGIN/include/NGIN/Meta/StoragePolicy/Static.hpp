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

	template <std::size_t Size = 64>
	class NGIN_STATIC_STORAGE_ALIGNMENT_ATTRIBUTE Static
	{
		static_assert(Size % NGIN_STATIC_STORAGE_ALIGNMENT == 0, "Size must be a multiple of " NGIN_STATIC_STORAGE_TO_STRING(NGIN_STATIC_STORAGE_ALIGNMENT) ".");

	public:
		/// @brief Default constructor.
		/// Initializes the internal storage.
		Static();

		/// @brief Copy constructor (deleted).
		/// StoragePolicies can only be moved.
		Static(const Static &other) = delete;

		/// @brief Move assignment operator.
		/// @param other The object to be moved.
		/// @return Reference to the moved object.
		Static &operator=(Static &&other) noexcept;

		/// @brief Move constructor.
		/// @param other The object to be moved.
		Static(Static &&other) noexcept;

		/// @brief Construct from a storage wrappable type.
		/// @tparam T Type of the object to be stored.
		/// @param obj The object to be stored.
		template <IsStorageWrappable T>
		Static(T &&obj)
			requires IsNotSame<Static<Size>, T>;

		/// @brief Destructor.
		/// Calls the destructor of the stored object.
		~Static();

		/// @brief Get a pointer to the internal buffer.
		/// @return Pointer to the internal buffer.
		inline void *get() { return &buffer[0]; }

	private:
		/// @brief Function pointer for object destruction.
		void (*destructorFunc)(void *) = nullptr;

		/// @brief Function pointer for moving object data.
		void (*moveFunc)(void *dst, void *src) = nullptr;

		/// @brief Internal buffer to hold object data.
		std::byte buffer[Size];
	};

	template <std::size_t Size>
	Static<Size>::Static()
		: destructorFunc(nullptr), buffer{}
	{
	}

	template <std::size_t Size>
	Static<Size> &Static<Size>::operator=(Static &&other) noexcept
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

	template <std::size_t Size>
	Static<Size>::Static(Static &&other) noexcept
		: buffer{}
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

	template <std::size_t Size>
	template <IsStorageWrappable T>
	Static<Size>::Static(T &&obj)
		requires IsNotSame<Static<Size>, T>
	{
		using StoredType = std::decay_t<T>;
		static_assert(sizeof(StoredType) <= Size, "Type too large for Static.");

		new (&buffer[0]) StoredType(std::move(obj));
		destructorFunc = [](void *ptr)
		{
			static_cast<StoredType *>(ptr)->~T();
		};
		if constexpr (std::is_move_assignable_v<StoredType>)
		{
			moveFunc = [](void *dst, void *src)
			{
				*static_cast<StoredType *>(dst) = std::move(*static_cast<StoredType *>(src));
			};
		}
	}

	template <std::size_t Size>
	Static<Size>::~Static()
	{
		if (destructorFunc)
			destructorFunc(&buffer[0]);
	}

}