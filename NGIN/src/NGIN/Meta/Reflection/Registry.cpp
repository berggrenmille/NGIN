#include <NGIN/Meta/Reflection/Registry.hpp>
#include <iostream>

namespace NGIN::Meta::Reflection
{
    Registry& Registry::GetInstance()
    {
        static Registry instance;
        return instance;
    }

    void Registry::AddClass(const Types::Class& classData)
    {
        classMap[classData.name] = classData;
        std::cout << "Class: " << classData.name << std::endl;
        std::cout << "Fields:" << classData.fields.size() << std::endl;
        for (const auto& field: classData.fields)
        {
            std::cout << "\t" << field.name << " : " << field.type << " @ " << field.offset << std::endl;
        }
        std::cout << "Functions:" << classData.functions.size() << std::endl;
        for (const auto& func: classData.functions)
        {
            std::cout << "\t" << func.name << " -> " << func.returnType << " " << (func.isConst ? "const" : "")
                      << std::endl;
        }
    }

    const Types::Class& Registry::GetClassFromString(const std::string& className)
    {
        return classMap[className];
    }

    void Registry::AddEnum(const Types::Enum& enumData)
    {
        enumMap[enumData.name] = enumData;

        std::cout << "Enum:" << enumData.name << std::endl;
        std::cout << "Enumerators:" << enumData.enumerators.size() << std::endl;
        for (const auto& enumerator: enumData.enumerators)
        {
            std::cout << "\t" << enumerator.first << " : " << enumerator.second << std::endl;
        }


    }

    const Types::Enum& Registry::GetEnumFromString(const String& enumName)
    {
        return enumMap[enumName];
    }


}
