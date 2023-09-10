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

    template <typename... Args>
    std::tuple<Args...> ExtractArgsIntoTuple(va_list &args)
    {
        return std::tuple<Args...>{va_arg(args, Args)...};
    }

    template <typename ClassT, typename... Args>
    static void MethodInvoker(void *context, void *instance, ...)
    {
        auto methodPtr = reinterpret_cast<void (ClassT::*)(Args...)>(context);

        va_list args;
        va_start(args, instance);

        std::tuple<Args...> argumentsTuple;

        // Unpack the arguments
        std::apply([&args](auto &...items)
                   { (..., (items = va_arg(args, decltype(items)))); },
                   argumentsTuple);

        // Use the arguments tuple to invoke the method on the instance
        std::apply([instance, methodPtr](auto &...items)
                   { (reinterpret_cast<ClassT *>(instance)->*methodPtr)(items...); },
                   argumentsTuple);

        va_end(args);
    }

    template <typename ClassT>
    class ClassRegistrar
    {
    public:
        static void *DefaultCtor(void *memoryPtr)
        {
            return new (memoryPtr) ClassT();
        }

        static void DefaultDtor(void *memoryPtr)
        {
            reinterpret_cast<ClassT *>(memoryPtr)->~ClassT();
        }

        ClassRegistrar()
        {

            classData.name = TypeName<ClassT>::value;
            classData.ctor = &ClassRegistrar<ClassT>::DefaultCtor;

            classData.dtor = &ClassRegistrar<ClassT>::DefaultDtor;
        }

        ClassRegistrar(const std::string &className)
        {
            classData.name = className;
            classData.ctor = &ClassRegistrar<ClassT>::DefaultCtor;

            classData.dtor = &ClassRegistrar<ClassT>::DefaultDtor;
        }

        ClassRegistrar<ClassT> &RegisterConstructor()
        {
            classData.ctor = [](void *memoryPtr)
            {
                new (memoryPtr) ClassT();
            };
            return *this;
        }

        ClassRegistrar<ClassT> &RegisterDestructor()
        {
            classData.dtor = [](void *memoryPtr)
            {
                reinterpret_cast<ClassT *>(memoryPtr)->~ClassT();
            };
            return *this;
        }

        template <typename FieldType>
        ClassRegistrar<ClassT> &RegisterProperty(const std::string &fieldName, FieldType ClassT::*fieldPtr)
        {
            Field fieldData;
            fieldData.name = fieldName;
            fieldData.type = TypeName<FieldType>::value;
            fieldData.offset = reinterpret_cast<std::size_t>(&(reinterpret_cast<ClassT *>(0)->*fieldPtr));

            classData.fields.push_back(fieldData);
            return *this;
        }

        template <typename ReturnType, typename... Args>
        ClassRegistrar<ClassT> &RegisterMethod(const std::string &methodName, ReturnType (ClassT::*methodPtr)(Args...))
        {
            Function methodData;

            methodData.name = methodName;

            methodData.returnType = TypeName<ReturnType>::value;
            methodData.isConst = false;

            // Extract argument types
            (methodData.argTypes.push_back(TypeName<Args>::value.data()), ...);

            // Create a lambda to wrap the method
            methodData.invoker = [methodPtr](void *instance, std::vector<std::any> &args) -> std::any
            {
                ClassT *classInstance = reinterpret_cast<ClassT *>(instance);
                if (args.size() != sizeof...(Args))
                    throw std::runtime_error("Incorrect number of arguments.");
                return InvokeMethod(classInstance, methodPtr, args);
            };

            classData.functions.push_back(methodData);
            return *this;
        }

        template <typename ReturnType, typename... Args>
        ClassRegistrar<ClassT> &RegisterMethod(const std::string &methodName, ReturnType (ClassT::*methodPtr)(Args...) const)
        {
            Function methodData;

            methodData.name = methodName;

            methodData.returnType = TypeName<ReturnType>::value;
            methodData.isConst = true;

            // Extract argument types
            (methodData.argTypes.push_back(TypeName<Args>::value.data()), ...);

            // Create a lambda to wrap the method
            methodData.invoker = [methodPtr](void *instance, std::vector<std::any> &args) -> std::any
            {
                ClassT *classInstance = reinterpret_cast<ClassT *>(instance);
                if (args.size() != sizeof...(Args))
                    throw std::runtime_error("Incorrect number of arguments.");
                return InvokeMethod(classInstance, methodPtr, args);
            };

            classData.functions.push_back(methodData);
            return *this;
        }

        ~ClassRegistrar()
        {
            Registry::GetInstance().AddClass(classData);
        }

    private:
        Class classData;

        template <typename ReturnType, std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassT *classInstance, ReturnType (ClassT::*methodPtr)(ArgTypes...), std::vector<std::any> &args)
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

        template <std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassT *classInstance, void (ClassT::*methodPtr)(ArgTypes...), std::vector<std::any> &args)
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
        template <typename ReturnType, std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassT *classInstance, ReturnType (ClassT::*methodPtr)(ArgTypes...) const, std::vector<std::any> &args)
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

        template <std::size_t Index = 0, typename... ArgTypes>
        static std::any InvokeMethod(ClassT *classInstance, void (ClassT::*methodPtr)(ArgTypes...) const, std::vector<std::any> &args)
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
}