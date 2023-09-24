#include <gtest/gtest.h>
#include <NGIN/Meta/TypeID.hpp>

using namespace NGIN::Meta;

TEST(TypeIDTests, Uniqueness)
{
    // Test that the IDs are unique for different types
    ASSERT_NE(TypeID<int>(), TypeID<double>());
    ASSERT_NE(TypeID<int>(), TypeID<char>());
    ASSERT_NE(TypeID<double>(), TypeID<char>());
}

TEST(TypeIDTests, Consistency)
{
    // Test that the IDs are consistent within the same program execution
    ASSERT_EQ(TypeID<int>(), TypeID<int>());
    ASSERT_EQ(TypeID<double>(), TypeID<double>());
    ASSERT_EQ(TypeID<char>(), TypeID<char>());
}

TEST(TypeIDTests, ComplexType)
{
    // Test that the IDs are unique for complex types
    ASSERT_NE(TypeID<int>(), TypeID<int*>());
    ASSERT_NE(TypeID<int>(), TypeID<const int>());
    ASSERT_NE(TypeID<int>(), TypeID<int&>());
    ASSERT_NE(TypeID<int>(), TypeID<int&&>());
}