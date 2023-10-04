#pragma once

#include <NGIN/Defines.hpp>
#include <NGIN/Util/Delegate.hpp>
#include <string>
#include <functional>
#include <vector>
#include <any>

namespace NGIN::Meta::Reflection::Types
{
    /// \brief Represents metadata for a function within the reflection system.
    struct Function
    {
        Function() = default;

        Function(const Function& other)
        {
            name = other.name;
            returnType = other.returnType;
            isConst = other.isConst;
            argTypes = other.argTypes;
            invoker = other.invoker;
        }

        Function(Function&&) = default;

        Function& operator=(const Function& other)
        {
            name = other.name;
            returnType = other.returnType;
            isConst = other.isConst;
            argTypes = other.argTypes;
            invoker = other.invoker;
            return *this;
        }

        Function& operator=(Function&&) = default;

        /// \name of the function.
        std::string name = "Unnamed Function";

        /// \type of value that the function returns, represented as a string.
        std::string returnType = "Unknown Type";

        /// \brief Flag indicating if the function is a const member function.
        /// Set to 'true' if the function is const, otherwise 'false'.
        bool isConst = false;

        /// \brief List of the types of the function's arguments.
        /// Each type is represented as a string.
        std::vector<std::string> argTypes {};

        /// \brief
        /// \details
        /// - The first parameter (void *) a pointer to an instance on which
        ///   the member function should be called (for global functions this will be nullptr).
        /// - The second parameter (std::vector<std::any> &) represents the list of
        ///   arguments to pass to the function.
        DynamicDelegate invoker {};
    };

}