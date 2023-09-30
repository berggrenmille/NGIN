#include <gtest/gtest.h>
#include <NGIN/Meta/Reflection/Registration.hpp>


using namespace NGIN::Meta::Reflection;

class DummyClass
{
public:
    int myField = 5;

    [[nodiscard]] int GetMyField()
    { return myField; }

    [[nodiscard]] int GetMyFieldConst() const
    { return myField; }
};

NGIN_REFLECTION_REGISTRATION()
{
    RegisterClass<DummyClass>()
            .RegisterConstructor()
            .RegisterProperty("myField", &DummyClass::myField)
            .RegisterMethod("GetMyField", &DummyClass::GetMyField)
            .RegisterMethod("GetMyFieldConst", &DummyClass::GetMyFieldConst);
}

TEST(ReflectionTests, RegisterClass)
{


    const Types::Class& classInfo = Registry::GetInstance().GetClassFromString("DummyClass");
    ASSERT_EQ(classInfo.name == "DummyClass", true);
    ASSERT_EQ(classInfo.fields.size() == 1, true);
    ASSERT_EQ(classInfo.functions.size() == 2, true);
}
