#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include <functional>
#include <any>

namespace NGIN::Meta::Reflection
{

    struct Field
    {
        std::string name;
        std::string type;
        size_t offset;
    };

    struct Function
    {
        std::string name;
        std::string returnType;
        bool isConst;
        std::vector<std::string> argTypes;
        std::function<std::any(void *, std::vector<std::any> &)> invoker;
    };
    struct Enum
    {
        std::string name;
        std::vector<std::pair<std::string, int>> values; // name and integer value of each enum item
    };

    using CtorFuncPtr = void *(*)(void *);
    using DtorFuncPtr = void (*)(void *);
    struct Class
    {
        std::string name = "";
        size_t size = 0;
        size_t alignment = alignof(std::max_align_t);
        CtorFuncPtr ctor = nullptr;
        DtorFuncPtr dtor = nullptr;
        std::vector<Field> fields;
        std::vector<Function> functions;

        void *ConstructInstance(void *memoryPtr)
        {
            return ctor(memoryPtr);
        }
    };

}
