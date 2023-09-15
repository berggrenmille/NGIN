#pragma once
#include "Concepts.hpp"
namespace NGIN::Meta::StoragePolicy
{
	/// \class Dynamic
	/// \brief A class for dynamic storage management.
	///
	/// The Dynamic class is a utility for dynamically managing storage.
	/// It supports dynamic allocation and deallocation of objects of different types.
	/// This is particularly useful for type erasure techniques where the type of the object
	/// is required to be decoupled from the interface.
	class Dynamic
	{
	public:
		/// \brief Default constructor is deleted to prevent empty initialization.
		Dynamic() = default;

		Dynamic(const Dynamic &) = delete;

		Dynamic(Dynamic &&other) noexcept;

		Dynamic &operator=(Dynamic &&other) noexcept;

		/// \brief Construct Dynamic by moving an object.
		/// \tparam T The type of the object.
		/// \param obj The object to be moved into storage.
		template <IsStorageWrappable T>
		Dynamic(T &&obj)
			requires IsNotSame<Dynamic, T>;

		/// \brief Destructor for Dynamic.
		///
		/// Calls the destructor function pointer to clean up the dynamically allocated object.
		~Dynamic();

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

	inline Dynamic::Dynamic(Dynamic &&other) noexcept
	{
		if (destructor)
			destructor(ptr);

		ptr = other.ptr;
		destructor = other.destructor;

		other.ptr = nullptr;
		other.destructor = nullptr;
	}
	inline Dynamic &Dynamic::operator=(Dynamic &&other) noexcept
	{
		if (destructor)
			destructor(ptr);

		ptr = other.ptr;
		destructor = other.destructor;

		other.ptr = nullptr;
		other.destructor = nullptr;

		return *this;
	}
	template <IsStorageWrappable T>
	Dynamic::Dynamic(T &&obj)
		requires IsNotSame<Dynamic, T>
	{
		using StoredType = std::decay_t<T>;
		ptr = new StoredType(std::forward<T>(obj));
		destructor = [](void *objPtr)
		{
			delete static_cast<StoredType *>(objPtr);
		};
	}

	inline Dynamic::~Dynamic()
	{
		if (destructor)
			destructor(ptr);
	}

	inline void *Dynamic::get()
	{
		return ptr;
	}
}
