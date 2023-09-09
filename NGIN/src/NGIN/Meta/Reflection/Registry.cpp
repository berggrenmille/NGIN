#include <NGIN/Meta/Reflection/Registry.hpp>
#include <iostream>

namespace NGIN::Meta::Reflection
{
    Registry &Registry::GetInstance()
    {
        static Registry instance;
        return instance;
    }

    void Registry::AddClass(const Class &classData)
    {
        classMap[classData.name] = classData;
        std::cout << classData.name << std::endl;
        std::cout << "Fields:" << classData.fields.size() << std::endl;
        for (const auto &field : classData.fields)
        {
            std::cout << "\t" << field.name << " : " << field.type << " @ " << field.offset << std::endl;
        }
        std::cout << "Functions:" << classData.functions.size() << std::endl;
        for (const auto &func : classData.functions)
        {
            std::cout << "\t" << func.name << " : " << func.returnType << std::endl;
        }
    }
}
