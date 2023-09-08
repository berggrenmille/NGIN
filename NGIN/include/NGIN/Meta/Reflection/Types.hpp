#pragma once
#include <string>
#include <vector>
#include <cstddef>
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
        void *address;
    };

    struct Class
    {
        std::string name;
        std::vector<Field> fields;
        std::vector<Function> functions;
    };

    struct Enum
    {
        std::string name;
        std::vector<std::pair<std::string, int>> values; // name and integer value of each enum item
    };
}