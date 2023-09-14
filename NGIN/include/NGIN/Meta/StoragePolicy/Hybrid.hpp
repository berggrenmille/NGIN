#pragma once
#include <cstddef>
#include <memory>
#include <cstring>
#include "Concepts.hpp"

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
	template <std::size_t Size = 64>
	class NGIN_HYBRID_STORAGE_ALIGNMENT_ATTRIBUTE Hybrid
	{
		static_assert(Size% NGIN_HYBRID_STORAGE_ALIGNMENT == 0, "Size must be a multiple of " NGIN_HYBRID_STORAGE_TO_STRING(NGIN_HYBRID_STORAGE_ALIGNMENT) ".");
		static_assert(Size >= sizeof(void*), "Size must be at least the size of a pointer.");

	public:
		Hybrid();
		Hybrid(const Hybrid&) = delete;
		Hybrid(Hybrid&& other) noexcept;
		Hybrid& operator=(Hybrid&& other) noexcept;

		template <IsStorageWrappable T>
		Hybrid(T&& obj) requires IsNotSame<Hybrid<Size>, T>;

		~Hybrid();

		void* get();

	private:
		void SetDestructorFunc(void (*d)(void*), bool heap);
		bool UseHeap() const;

		void (*destructorFunc)(void*) = nullptr;
		void (*moveFunc)(void* dst, void* src) = nullptr;


		union Data
		{
			void* ptr = nullptr;
			std::byte buffer[Size];
			Data() : buffer{  } {}
		} data;
	};

	template <std::size_t Size>
	Hybrid<Size>::Hybrid()
		: data{}
	{}

	template <std::size_t Size>
	Hybrid<Size>::Hybrid(Hybrid&& other) noexcept
		: data{}
	{
		if (!UseHeap())
		{
			if (other.moveFunc)
				other.moveFunc(get(), other.get());
			else
				std::memcpy(get(), other.get(), Size);
			destructorFunc = other.destructorFunc;
			moveFunc = other.moveFunc;

			other.destructorFunc = nullptr;
		}
		else
		{
			data.ptr = other.data.ptr;
			destructorFunc = other.destructorFunc;

			other.data.ptr = nullptr;
			other.destructorFunc = nullptr;
		}
	}

	template <std::size_t Size>
	Hybrid<Size>& Hybrid<Size>::operator=(Hybrid&& other) noexcept
	{
		if (!UseHeap())
		{
			if (other.moveFunc)
				other.moveFunc(get(), other.get());
			else
				std::memcpy(get(), other.get(), Size);
			destructorFunc = other.destructorFunc;
			moveFunc = other.moveFunc;

			other.destructorFunc = nullptr;
		}
		else
		{
			data.ptr = other.data.ptr;
			destructorFunc = other.destructorFunc;

			other.data.ptr = nullptr;
			other.destructorFunc = nullptr;
		}
		return *this;
	}

	template <std::size_t Size>
	template <IsStorageWrappable T>
	Hybrid<Size>::Hybrid(T&& obj) requires IsNotSame<Hybrid<Size>, T>
	{
		using StoredType = std::decay_t<T>;
		if constexpr (sizeof(StoredType) <= Size)
		{
			// Using placement new here
			new (&data.buffer[0]) StoredType(std::move(obj));
			SetDestructorFunc([](void* obj)
							  { static_cast<StoredType*>(obj)->~T(); },
							  false);
		}
		else
		{
			data.ptr = new StoredType(std::move(obj));
			SetDestructorFunc([](void* obj)
							  { delete static_cast<StoredType*>(obj); },
							  true);
		}

		if constexpr (std::is_move_assignable_v<StoredType>)
		{
			moveFunc = [](void* dst, void* src)
			{
				*static_cast<StoredType*>(dst) = std::move(*static_cast<StoredType*>(src));
			};
		}
	}

	template <std::size_t Size>
	Hybrid<Size>::~Hybrid()
	{
		void (*actualDestructor)(void*) = reinterpret_cast<void (*)(void*)>(reinterpret_cast<std::uintptr_t>(destructorFunc) & ~std::uintptr_t(1));

		if (actualDestructor)
		{
			actualDestructor(UseHeap() ? data.ptr : &data.buffer[0]);
		}

	}

	template <std::size_t Size>
	void* Hybrid<Size>::get()
	{
		return UseHeap() ? data.ptr : &data.buffer[0];
	}

	template <std::size_t Size>
	void Hybrid<Size>::SetDestructorFunc(void (*d)(void*), bool heap)
	{
		destructorFunc = reinterpret_cast<void (*)(void*)>((reinterpret_cast<uintptr_t>(d) & ~uintptr_t(1)) | static_cast<uintptr_t>(heap));
	}

	template <std::size_t Size>
	bool Hybrid<Size>::UseHeap() const
	{
		return reinterpret_cast<uintptr_t>(destructorFunc) & 1;
	}
}
