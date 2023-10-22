#pragma once

#include <NGIN/Defines.hpp>

#include <NGIN/Meta/TypeWrapper.hpp>


namespace NGIN::Core
{
    // Forward declarations
    class Engine;

    /// @class Module
    /// @brief Base class for all modules
    /// @details Modules are the building blocks of the engine.
    ///          They are used to extend the engine's functionality.
    ///          Modules can declare dependencies by overriding the NGIN::Core::Module::Dependencies alias
    ///          The engine will then make sure that all dependencies are initialized before the module itself.
    ///          Circular dependencies are not support and will result in undefined behavior.
    ///          Modules are initialized in the order of their dependencies.
    ///          Modules are shutdown in the reverse order of their dependencies.
    ///          It is recommended to loosely couple modules by using Events instead of using the modules directly.
    class Module
    {
    public:
        friend class Engine;

        /// @brief Alias for declaring dependencies
        /// @details Example:
        /// using Dependencies = Meta::TypeWrapper<Module1, Module2, ...>;
        using Dependencies = Meta::TypeWrapper<void>;


        /// @brief Default constructor
        NGIN_API Module() = default;
        /// @brief Default destructor
        NGIN_API virtual ~Module() = default;

    protected:
        /// @brief Called when the module is initialized
        NGIN_API virtual void OnInit(Engine* engine) {}
        /// @brief Called when the module is shutdown
        NGIN_API virtual void OnShutdown() {}
        /// @brief Called before the module is ticked
        NGIN_API virtual void OnPreTick(const F64 deltaTime) {}
        /// @brief Called when the module is ticked
        NGIN_API virtual void OnTick(const F64 deltaTime) {}
        /// @brief Called after the module is ticked
        NGIN_API virtual void OnPostTick(const F64 deltaTime) {}


    };
}