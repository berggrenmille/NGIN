#pragma once
#include <Core/Core.h>
#include <type_traits>

namespace NGIN
{
	/**
	 * @class App
	 * @brief A base class for NGIN applications.
	 *
	 * This class is meant to be inherited by any class that represents an NGIN application.
	 */
	class NGIN_API App
	{
	public:
		/// Default constructor.
		App();

		/// Destructor.
		virtual ~App();

		/**
		 * @brief Initializes the application.
		 *
		 * This function should be overridden by derived classes to include any necessary initialization logic.
		 */
		virtual void Init();
	};

	/**
	 * @brief Checks if a type is an App.
	 *
	 * @tparam T Type to check.
	 */
	template<class T>
	concept is_app = std::is_base_of<App, T>::value;

	/**
	 * @brief Checks if a type is a pointer to an App.
	 *
	 * @tparam T Type to check.
	 */
	template<typename T>
	concept is_app_ptr = std::is_base_of<App, std::remove_pointer_t<T>>::value;
};
