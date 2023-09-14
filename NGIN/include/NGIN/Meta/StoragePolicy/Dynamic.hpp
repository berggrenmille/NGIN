#pragma once

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
		Dynamic() = delete;


		/// \brief Construct Dynamic with an object.
		/// \tparam T The type of the object.
		/// \param obj The object to be stored.
		template <typename T>
		Dynamic(const T& obj)
		{
			using StoredType = std::decay_t<T>;

			if constexpr (std::is_same<StoredType, Dynamic>::value)
			{
				ptr = obj.ptr;
				destructor = obj.destructor;
			}
			else
			{
				ptr = new StoredType(obj);
				destructor = [](void* obj)
				{
					delete static_cast<StoredType*>(obj);
				};
			}
		}

		/// \brief Construct Dynamic by moving an object.
		/// \tparam T The type of the object.
		/// \param obj The object to be moved into storage.
		template <typename T>
		Dynamic(T&& obj)
		{
			using StoredType = std::decay_t<T>;
			if constexpr (std::is_same<StoredType, Dynamic>::value)
			{
				ptr = obj.ptr;
				destructor = obj.destructor;

				obj.ptr = nullptr;
				obj.destructor = nullptr;
			}
			else
			{

				ptr = new StoredType(std::forward<T>(obj));
				destructor = [](void* objPtr)
				{
					delete static_cast<StoredType*>(objPtr);
				};
			}
		}



		/// \brief Destructor for Dynamic.
		///
		/// Calls the destructor function pointer to clean up the dynamically allocated object.
		~Dynamic()
		{
			if (destructor)
				destructor(ptr);
		}

		/// \brief Get the stored object as a void pointer.
		/// \return A void pointer to the stored object.
		void* get() const { return ptr; }

	private:
		/// \brief A pointer to the stored object.
		void* ptr = nullptr;

		/// \brief A function pointer for the destructor of the stored object.
		///
		/// This function pointer will point to the appropriate destructor based on
		/// the type of object stored. It is used in the destructor to clean up the object.
		void (*destructor)(void*) = nullptr;
	};
}
