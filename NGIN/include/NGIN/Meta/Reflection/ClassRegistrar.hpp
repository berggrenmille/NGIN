#pragma once

#include "Types.hpp"
#include <type_traits>
#include <typeinfo>
#include "Registry.hpp"

namespace NGIN::Meta::Reflection
{

    class ClassRegistrar
    {
    public:
        ClassRegistrar(const std::string &className)
        {
            classData.name = className;
        }

        template <typename ClassType, typename FieldType>
        ClassRegistrar &RegisterProperty(const std::string &fieldName, FieldType ClassType::*fieldPtr)
        {
            Field fieldData;
            fieldData.name = fieldName;
            fieldData.type = typeid(FieldType).name();
            fieldData.offset = reinterpret_cast<std::size_t>(&(reinterpret_cast<ClassType *>(0)->*fieldPtr));

            classData.fields.push_back(fieldData);
            return *this;
        }

        template <typename ClassType, typename ReturnType, typename... Args>
        ClassRegistrar &RegisterMethod(const std::string &methodName, ReturnType (ClassType::*methodPtr)(Args...))
        {
            Function funcData;
            funcData.name = methodName;
            funcData.returnType = typeid(ReturnType).name();
            funcData.address = reinterpret_cast<void *>(methodPtr);

            classData.functions.push_back(funcData);
            return *this;
        }

        ~ClassRegistrar()
        {

            Registry::GetInstance().AddClass(classData);
        }

    private:
        Class classData;
    };
}