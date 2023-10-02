#pragma once

// EnumRegistrar.hpp
#include <NGIN/Defines.hpp>
#include <NGIN/Meta/Reflection/Types/Enum.hpp>
#include <NGIN/Meta/TypeName.hpp>
#include "Registry.hpp"


#include <unordered_map>

namespace NGIN::Meta::Reflection
{

    /// \brief Contains metadata and functionalities to register enumerations.
    template<typename EnumType>
    class EnumRegistrar
    {
    public:
        /// \brief Default constructor.
        EnumRegistrar();

        /// \brief Constructor that accepts a name for the enumeration.
        /// \param enumName The name to set for the enumeration.
        EnumRegistrar(const String& enumName);

        /// \brief Registers an enumerator.
        /// \param name The name of the enumerator.
        /// \param value The value of the enumerator.
        EnumRegistrar<EnumType>& RegisterEnumerator(const String& name, EnumType value);

        /// \brief Destructor
        ~EnumRegistrar();

    private:

        /// \brief The metadata about the enumeration
        Types::Enum enumData {};


    };

    template<typename EnumType>
    EnumRegistrar<EnumType>::EnumRegistrar()
    {
        enumData.name = TypeName<EnumType>::Class();
        // Init the enumData structure here if needed
    }

    template<typename EnumType>
    EnumRegistrar<EnumType>::EnumRegistrar(const String& enumName)
    {
        enumData.name = enumName;
    }

    template<typename EnumType>
    EnumRegistrar<EnumType>& EnumRegistrar<EnumType>::RegisterEnumerator(const String& name, EnumType value)
    {
        enumData.enumerators[value] = name;
        return *this;
    }

    template<typename EnumType>
    EnumRegistrar<EnumType>::~EnumRegistrar()
    {
        Registry::GetInstance().AddEnum(enumData);
    }


}