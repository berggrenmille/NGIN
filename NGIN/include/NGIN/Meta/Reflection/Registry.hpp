#pragma once
#include "Types.hpp"
#include <iostream>
namespace NGIN::Meta::Reflection
{
    class Registry
    {
    public:
        static Registry &GetInstance()
        {
            static Registry instance;
            return instance;
        }

        void AddClass(const Class &classData)
        {
            classes.push_back(classData);
            std::cout << classData.name << std::endl;
            std::cout << "Fields:" << std::endl;
            for (const auto &field : classData.fields)
            {
                std::cout << "\t" << field.name << " : " << field.type << " @ " << field.offset << std::endl;
            }
            std::cout << "Functions:" << std::endl;
            for (const auto &func : classData.functions)
            {
                std::cout << "\t" << func.name << " : " << func.returnType << " @ " << func.address << std::endl;
            }
        }

    private:
        std::vector<Class> classes;
    };
}
