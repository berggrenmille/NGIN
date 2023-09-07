#pragma once

#include <NGIN/Meta/DynamicStoragePolicy.hpp>
#include <NGIN/Memory/Allocator.hpp>

#include <memory>
#include <type_traits>
#include <utility>
#include <concepts>

namespace NGIN
{

	/**
	 * @namespace Internal
	 * @brief Contains internal concepts to check for expected behaviors.
	 */
	namespace Internal
	{

		/**
		 * @brief Checks if the given type has an OnAttach() method.
		 * @tparam T Type to check.
		 */
		template <typename T>
		concept HasOnAttach = requires(T a) {
			{
				a.OnAttach()
			} -> std::same_as<void>;
		};

		/**
		 * @brief Checks if the given type has an OnDetach() method.
		 * @tparam T Type to check.
		 */
		template <typename T>
		concept HasOnDetach = requires(T a) {
			{
				a.OnDetach()
			} -> std::same_as<void>;
		};

		/**
		 * @brief Checks if the given type has an OnUpdate() method.
		 * @tparam T Type to check.
		 */
		template <typename T>
		concept HasOnUpdate = requires(T a) {
			{
				a.OnUpdate()
			} -> std::same_as<void>;
		};

	} // namespace Internal

	/**
	 * @class Layer
	 * @brief Represents a generic layer within the NGIN game engine.
	 *
	 * This class can manage any object that matches the expected interface (OnAttach, OnDetach, OnUpdate).
	 * Instead of relying on virtual functions, it utilizes manual vtable pointers for a dynamic dispatch mechanism.
	 */
	template <typename StoragePolicy = Meta::DynamicStoragePolicy>
	class Layer
	{
	public:
		/**
		 * @brief Construct a new Layer object.
		 *
		 * @tparam T Type of the layer object.
		 * @param layer The layer object.
		 */
		template <typename T>
		Layer(T &&layer)
			: pimpl(std::move(layer))
		{
			SetupFunctionPointers<T>();
		}

		template <typename T>
		Layer(T &&layer, Memory::Allocator<> &allocator)
			: pimpl(std::move(layer), allocator)
		{
			SetupFunctionPointers<T>();
		}

		/**
		 * @brief Invoke the OnAttach method of the stored object.
		 */
		void OnAttach() { onAttach(pimpl.get()); }

		/**
		 * @brief Invoke the OnDetach method of the stored object.
		 */
		void OnDetach() { onDetach(pimpl.get()); }

		/**
		 * @brief Invoke the OnUpdate method of the stored object.
		 */
		void OnUpdate() { onUpdate(pimpl.get()); }

	private:
		/**
		 * @brief A unique pointer that manages the dynamic pimpl of the object.
		 *
		 * Utilizes a custom deleter to ensure the correct destruction of the type-erased object.
		 */
		StoragePolicy pimpl;

		/**
		 * @brief Function pointer that triggers the OnAttach method of the stored object.
		 */
		void (*onAttach)(void *) = nullptr;

		/**
		 * @brief Function pointer that triggers the OnDetach method of the stored object.
		 */
		void (*onDetach)(void *) = nullptr;

		/**
		 * @brief Function pointer that triggers the OnUpdate method of the stored object.
		 */
		void (*onUpdate)(void *) = nullptr;

		/**
		 * @brief Initialize function pointers based on the provided type.
		 *
		 * This method determines the correct lambda functions for the manual dispatch
		 * of the interface methods based on the type provided.
		 *
		 * @tparam T The type of the object managed by this layer.
		 */
		template <typename T>
		void SetupFunctionPointers()
		{
			onAttach = [](void *obj)
			{
				if constexpr (Internal::HasOnAttach<T>)
				{
					reinterpret_cast<T *>(obj)->OnAttach();
				}
			};

			onDetach = [](void *obj)
			{
				if constexpr (Internal::HasOnDetach<T>)
				{
					reinterpret_cast<T *>(obj)->OnDetach();
				}
			};

			onUpdate = [](void *obj)
			{
				if constexpr (Internal::HasOnUpdate<T>)
				{
					reinterpret_cast<T *>(obj)->OnUpdate();
				}
			};
		}
	};

} // namespace NGIN
