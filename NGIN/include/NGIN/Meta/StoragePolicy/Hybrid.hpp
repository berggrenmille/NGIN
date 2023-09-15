#pragma once
#include <cstddef>
#include <memory>
#include <cstring>
#include "Concepts.hpp"
#include <assert.h>

#define NGIN_HYBRID_STORAGE_ALIGNMENT 8
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
	template <std::size_t Size = 64>
	class NGIN_HYBRID_STORAGE_ALIGNMENT_ATTRIBUTE Hybrid
	{
		static_assert(Size % NGIN_HYBRID_STORAGE_ALIGNMENT == 0, "Size must be a multiple of " NGIN_HYBRID_STORAGE_TO_STRING(NGIN_HYBRID_STORAGE_ALIGNMENT) ".");

	public:
		/// \brief Default constructor.
		Hybrid();

		/// \brief Copy constructor is deleted.
		Hybrid(const Hybrid &) = delete;

		/// \brief Move constructor.
		/// \param other The object to move from.
		Hybrid(Hybrid &&other) noexcept;

		/// \brief Move assignment operator.
		/// \param other The object to move from.
		/// \return A reference to the moved-to object.
		Hybrid &operator=(Hybrid &&other) noexcept;

		/// \brief Constructor taking a wrappable storage type.
		/// \tparam T Type of the object to store.
		/// \param obj The object to store.
		template <IsStorageWrappable T>
		Hybrid(T &&obj)
			requires IsNotSame<Hybrid<Size>, T>;

		/// \brief Destructor.
		~Hybrid();

		/// \brief Retrieve the pointer to the stored data.
		/// \return A pointer to the stored data.
		void *get();

	private:
		/// \union Data
		/// \brief Union for either holding a pointer to heap-allocated data or a static buffer.
		union Data
		{
			/// \brief Pointer to heap-allocated data.
			void *ptr;

			/// \brief Static buffer for data.
			std::byte buffer[Size];

			/// \brief Default constructor for the union which initializes the buffer.
			Data() : buffer{} {}
		} data;

		/// \brief Pointer to the function responsible for destructing the stored object.
		void (*destructorFunc)(void *) = nullptr;

		/// \brief Pointer to the function responsible for moving the stored object.
		void (*moveFunc)(void *dst, void *src) = nullptr;

		/// \brief Flag indicating whether the stored object is heap-allocated.
		bool useHeap = false;
	};

	template <std::size_t Size>
	Hybrid<Size>::Hybrid()
		: data{}
	{
	}

	template <std::size_t Size>
	Hybrid<Size>::Hybrid(Hybrid &&other) noexcept
		: data{}
	{
		if (this == &other)
			return;

		destructorFunc = other.destructorFunc;
		moveFunc = other.moveFunc;
		useHeap = other.useHeap;

		if (useHeap)
		{
			data.ptr = other.data.ptr;
			other.data.ptr = nullptr;
		}
		else if (moveFunc)
		{
			moveFunc(get(), other.get());
		}
		else
		{
			std::memcpy(get(), other.get(), Size);
		}

		other.data.ptr = nullptr;
		other.destructorFunc = nullptr;
	}

	template <std::size_t Size>
	Hybrid<Size> &Hybrid<Size>::operator=(Hybrid &&other) noexcept
	{
		if (this == &other)
			return *this;

		destructorFunc = other.destructorFunc;
		moveFunc = other.moveFunc;
		useHeap = other.useHeap;

		if (useHeap)
		{
			data.ptr = other.data.ptr;
			other.data.ptr = nullptr;
		}
		else if (moveFunc)
		{
			moveFunc(get(), other.get());
		}
		else
		{
			std::memcpy(get(), other.get(), Size);
		}

		other.data.ptr = nullptr;
		other.destructorFunc = nullptr;
		return *this;
	}

	template <std::size_t Size>
	template <IsStorageWrappable T>
	Hybrid<Size>::Hybrid(T &&obj)
		requires IsNotSame<Hybrid<Size>, T>
	{
		using StoredType = std::decay_t<T>;
		if constexpr (sizeof(StoredType) <= Size)
		{
			// Using placement new here
			new (&data.buffer[0]) StoredType(std::move(obj));
			destructorFunc = [](void *obj)
			{ static_cast<StoredType *>(obj)->~T(); };
			useHeap = false;
		}
		else
		{
			data.ptr = new StoredType(std::move(obj));
			destructorFunc = [](void *obj)
			{ delete static_cast<StoredType *>(obj); };
			useHeap = true;
		}

		if constexpr (std::is_move_assignable_v<StoredType>)
		{
			moveFunc = [](void *dst, void *src)
			{
				*static_cast<StoredType *>(dst) = std::move(*static_cast<StoredType *>(src));
			};
		}
	}

	template <std::size_t Size>
	Hybrid<Size>::~Hybrid()
	{

		if (destructorFunc)
		{
			destructorFunc(get());
		}
	}

	template <std::size_t Size>
	void *Hybrid<Size>::get()
	{
		return useHeap ? data.ptr : &data.buffer[0];
	}

}
