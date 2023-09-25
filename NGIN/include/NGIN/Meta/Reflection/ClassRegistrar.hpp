#pragma once

#include "Types.hpp"
#include <type_traits>
#include <typeinfo>
#include "Registry.hpp"
#include <NGIN/Meta/TypeName.hpp>
#include <stdarg.h>
#include <tuple>
namespace NGIN::Meta::Reflection
{

    template <typename ClassType>
    class ClassRegistrar
    {
    public:
        static void* DefaultCtor(void* memoryPtr);
        static void DefaultDtor(void* memoryPtr);

        ClassRegistrar();
        ClassRegistrar(const std::string& className);

        ClassRegistrar<ClassType>& RegisterConstructor();
        ClassRegistrar<ClassType>& RegisterDestructor();

        template <typename FieldType>
        ClassRegistrar<ClassType>& RegisterProperty(const std::string& fieldName, FieldType ClassType::* fieldPtr);

        template <typename ReturnType, typename... Args>
        ClassRegistrar<ClassType>& RegisterMethod(const std::string& methodName, ReturnType(ClassType::* methodPtr)(Args...));

        template <typename ReturnType, typename... Args>
        ClassRegistrar<ClassType>& RegisterMethod(const std::string& methodName, ReturnType(ClassType::* methodPtr)(Args...) const);

        ~ClassRegistrar();

    private:
        Types::Class classData;

        template <typename ReturnType, std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassType* classInstance, ReturnType(ClassType::* methodPtr)(ArgTypes...), std::vector<std::any>& args);

        template <std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassType* classInstance, void (ClassType::* methodPtr)(ArgTypes...), std::vector<std::any>& args);

        template <typename ReturnType, std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassType* classInstance, ReturnType(ClassType::* methodPtr)(ArgTypes...) const, std::vector<std::any>& args);

        template <std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassType* classInstance, void (ClassType::* methodPtr)(ArgTypes...) const, std::vector<std::any>& args);
    };

    template <typename ClassType>
    void* ClassRegistrar<ClassType>::DefaultCtor(void* memoryPtr)
    {
        return new (memoryPtr) ClassType();
    }

    template <typename ClassType>
    void ClassRegistrar<ClassType>::DefaultDtor(void* memoryPtr)
    {
        reinterpret_cast<ClassType*>(memoryPtr)->~ClassType();
    }

    template <typename ClassType>
    ClassRegistrar<ClassType>::ClassRegistrar()
    {
        classData.name = TypeName<ClassType>::value;
        classData.ctor = &ClassRegistrar<ClassType>::DefaultCtor;

        classData.dtor = &ClassRegistrar<ClassType>::DefaultDtor;
    }

    template <typename ClassType>
    ClassRegistrar<ClassType>::ClassRegistrar(const std::string& className)
    {
        classData.name = className;
        classData.ctor = &ClassRegistrar<ClassType>::DefaultCtor;

        classData.dtor = &ClassRegistrar<ClassType>::DefaultDtor;
    }

    template <typename ClassType>
    ClassRegistrar<ClassType>& ClassRegistrar<ClassType>::RegisterConstructor()
    {
        classData.ctor = [](void* memoryPtr)
            {
                new (memoryPtr) ClassType();
            };
        return *this;
    }

    template <typename ClassType>
    ClassRegistrar<ClassType>& ClassRegistrar<ClassType>::RegisterDestructor()
    {
        classData.dtor = [](void* memoryPtr)
            {
                reinterpret_cast<ClassType*>(memoryPtr)->~ClassType();
            };
        return *this;
    }

    template <typename ClassType>
    template <typename FieldType>
    ClassRegistrar<ClassType>& ClassRegistrar<ClassType>::RegisterProperty(const std::string& fieldName, FieldType ClassType::* fieldPtr)
    {
        Types::Field fieldData;
        fieldData.name = fieldName;
        fieldData.type = TypeName<FieldType>::value;
        fieldData.offset = reinterpret_cast<std::size_t>(&(reinterpret_cast<ClassType*>(0)->*fieldPtr));

        classData.fields.push_back(fieldData);
        return *this;
    }

    template <typename ClassType>
    template <typename ReturnType, typename... Args>
    ClassRegistrar<ClassType>& ClassRegistrar<ClassType>::RegisterMethod(const std::string& methodName, ReturnType(ClassType::* methodPtr)(Args...))
    {
        Types::Function methodData;

        methodData.name = methodName;

        methodData.returnType = TypeName<ReturnType>::value;
        methodData.isConst = false;

        // Extract argument types
        (methodData.argTypes.push_back(TypeName<Args>::value.data()), ...);

        // Create a lambda to wrap the method
        methodData.invoker = [methodPtr](void* instance, std::vector<std::any>& args) -> std::any
            {
                ClassType* classInstance = reinterpret_cast<ClassType*>(instance);
                if (args.size() != sizeof...(Args))
                    throw std::runtime_error("Incorrect number of arguments.");
                return InvokeMethod(classInstance, methodPtr, args);
            };

        classData.functions.push_back(methodData);
        return *this;
    }

    template <typename ClassType>
    template <typename ReturnType, typename... Args>
    ClassRegistrar<ClassType>& ClassRegistrar<ClassType>::RegisterMethod(const std::string& methodName, ReturnType(ClassType::* methodPtr)(Args...) const)
    {
        Types::Function methodData;

        methodData.name = methodName;

        methodData.returnType = TypeName<ReturnType>::value;
        methodData.isConst = true;

        // Extract argument types
        (methodData.argTypes.push_back(TypeName<Args>::value.data()), ...);

        // Create a lambda to wrap the method
        methodData.invoker = [methodPtr](void* instance, std::vector<std::any>& args) -> std::any
            {
                ClassType* classInstance = reinterpret_cast<ClassType*>(instance);
                if (args.size() != sizeof...(Args))
                    throw std::runtime_error("Incorrect number of arguments.");
                return InvokeMethod(classInstance, methodPtr, args);
            };

        classData.functions.push_back(methodData);
        return *this;
    }

    template <typename ClassType>
    ClassRegistrar<ClassType>::~ClassRegistrar()
    {
        Registry::GetInstance().AddClass(classData);
    }

    template <typename ClassType>
    template <typename ReturnType, std::size_t Index, typename... ArgTypes>
    std::any ClassRegistrar<ClassType>::InvokeMethod(ClassType* classInstance, ReturnType(ClassType::* methodPtr)(ArgTypes...), std::vector<std::any>& args)
    {
        if constexpr (Index == sizeof...(ArgTypes)) // Base case
        {
            return std::any(std::apply([classInstance, methodPtr](auto &&...unpackedArgs)
                { return (classInstance->*methodPtr)(std::forward<decltype(unpackedArgs)>(unpackedArgs)...); },
                std::tuple<std::any_cast<ArgTypes>(args[Index])...>));
        }
        else // This static_assert is to ensure the function won't compile when the index goes out of range
        {
            static_assert(Index < sizeof...(ArgTypes), "Index out of range when unpacking arguments.");
            return {};
        }
    }

    template <typename ClassType>
    template <std::size_t Index, typename... ArgTypes>
    std::any ClassRegistrar<ClassType>::InvokeMethod(ClassType* classInstance, void (ClassType::* methodPtr)(ArgTypes...), std::vector<std::any>& args)
    {
        if constexpr (Index == sizeof...(ArgTypes)) // Base case
        {
            auto tupleArgs = std::make_tuple(std::any_cast<ArgTypes>(args[Index])...);
            std::apply([classInstance, methodPtr](auto &&...unpackedArgs)
                { (classInstance->*methodPtr)(std::forward<decltype(unpackedArgs)>(unpackedArgs)...); },
                tupleArgs);
            return {};
        }
        else // This static_assert is to ensure the function won't compile when the index goes out of range
        {
            static_assert(Index < sizeof...(ArgTypes), "Index out of range when unpacking arguments.");
            return {};
        }
    }

    // INVOKE
    template <typename ClassType>
    template <typename ReturnType, std::size_t Index, typename... ArgTypes>
    std::any ClassRegistrar<ClassType>::InvokeMethod(ClassType* classInstance, ReturnType(ClassType::* methodPtr)(ArgTypes...) const, std::vector<std::any>& args)
    {
        if constexpr (Index == sizeof...(ArgTypes)) // Base case
        {
            return std::any(std::apply([classInstance, methodPtr](auto &&...unpackedArgs)
                { return (classInstance->*methodPtr)(std::forward<decltype(unpackedArgs)>(unpackedArgs)...); },
                std::tuple<std::any_cast<ArgTypes>(args[Index])...>));
        }
        else // This static_assert is to ensure the function won't compile when the index goes out of range
        {
            static_assert(Index < sizeof...(ArgTypes), "Index out of range when unpacking arguments.");
            return {};
        }
    }

    template <typename ClassType>
    template <std::size_t Index, typename... ArgTypes>
    std::any ClassRegistrar<ClassType>::InvokeMethod(ClassType* classInstance, void (ClassType::* methodPtr)(ArgTypes...) const, std::vector<std::any>& args)
    {
        if constexpr (Index == sizeof...(ArgTypes)) // Base case
        {
            auto tupleArgs = std::make_tuple(std::any_cast<ArgTypes>(args[Index])...);
            std::apply([classInstance, methodPtr](auto &&...unpackedArgs)
                { (classInstance->*methodPtr)(std::forward<decltype(unpackedArgs)>(unpackedArgs)...); },
                tupleArgs);
            return {};
        }
        else // This static_assert is to ensure the function won't compile when the index goes out of range
        {
            static_assert(Index < sizeof...(ArgTypes), "Index out of range when unpacking arguments.");
            return {};
        }
    }
};
