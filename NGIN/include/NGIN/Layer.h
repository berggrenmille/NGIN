#pragma once
#include <memory>
#include <type_traits>
#include <utility>
#include <concepts>
namespace NGIN
{
	namespace detail
	{
		/**
		 * @brief Concept to check for the presence of OnAttach() method.
		 */
		template <typename T>
		concept HasOnAttach = requires(T a) {
			{
				a.OnAttach()
			} -> std::same_as<void>;
		};

		/**
		 * @brief Concept to check for the presence of OnDetach() method.
		 */
		template <typename T>
		concept HasOnDetach = requires(T a) {
			{
				a.OnDetach()
			} -> std::same_as<void>;
		};

		/**
		 * @brief Concept to check for the presence of OnUpdate() method.
		 */
		template <typename T>
		concept HasOnUpdate = requires(T a) {
			{
				a.OnUpdate()
			} -> std::same_as<void>;
		};
	}

	/**
	 * @class Layer
	 * @brief Represents a generic layer within the NGIN game engine.
	 *
	 * This class is type-erased and can handle any object that conforms to the expected
	 * interface (OnAttach, OnDetach, OnUpdate). It provides a dynamic dispatch mechanism
	 * without relying on virtual functions, using manual vtable pointers.
	 */
	class Layer
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Layer() = default;

		/**
		 * @brief Copy constructor that accepts an instance of any conforming type.
		 *
		 * @param instance The object to be copied into the layer.
		 */
		template <typename T>
		explicit Layer(const T &instance) // For lvalues and const objects
		{
			T *object = new T(instance); // copy
			setupFunctionPointers<T>();
			storage = std::unique_ptr<void, void (*)(void *)>(object, [](void *ptr)
															  { delete static_cast<T *>(ptr); });
		}

		/**
		 * @brief Move constructor that accepts an instance of any conforming type.
		 *
		 * @param instance The object to be moved into the layer.
		 */
		template <typename T>
		explicit Layer(T &&instance) // For rvalues (temporary objects or std::move)
		{
			T *object = new T(std::move(instance)); // move
			setupFunctionPointers<T>();
			storage = std::unique_ptr<void, void (*)(void *)>(object, [](void *ptr)
															  { delete static_cast<T *>(ptr); });
		}

		/**
		 * @brief Invokes the OnAttach method of the stored object.
		 */
		void OnAttach() { onAttach(&storage); }
		/**
		 * @brief Invokes the OnDetach method of the stored object.
		 */
		void OnDetach() { onDetach(&storage); }
		/**
		 * @brief Invokes the OnUpdate method of the stored object.
		 */
		void OnUpdate() { onUpdate(&storage); }

	private:
		/**
		 * @brief Unique pointer that manages the dynamic storage of the object.
		 *
		 * It utilizes a custom deleter to properly destroy the type-erased object.
		 */
		std::unique_ptr<void, void (*)(void *)> storage;

		/**
		 * @brief Pointer to a function that invokes the OnAttach method of the stored object.
		 */
		void (*onAttach)(void *) = nullptr;
		/**
		 * @brief Pointer to a function that invokes the OnDetach method of the stored object.
		 */
		void (*onDetach)(void *) = nullptr;
		/**
		 * @brief Pointer to a function that invokes the OnUpdate method of the stored object.
		 */
		void (*onUpdate)(void *) = nullptr;

		/**
		 * @brief Sets up the function pointers based on the type provided.
		 *
		 * This method uses the type provided to instantiate the correct lambda functions
		 * that will be used for manual dispatch of the interface methods.
		 *
		 * @tparam T The type of the object being stored in this layer.
		 */
		template <typename T>
		void SetupFunctionPointers()
		{
			onAttach = [](void *obj)
			{
				if constexpr (detail::HasOnAttach<T>)
				{
					reinterpret_cast<T *>(obj)->OnAttach();
				}
			};

			onDetach = [](void *obj)
			{
				if constexpr (detail::HasOnDetach<T>)
				{
					reinterpret_cast<T *>(obj)->OnDetach();
				}
			};

			onUpdate = [](void *obj)
			{
				if constexpr (detail::HasOnUpdate<T>)
				{
					reinterpret_cast<T *>(obj)->OnUpdate();
				}
			};
		}
	};
}
