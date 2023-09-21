#include <gtest/gtest.h>
#include <NGIN/Util/Delegate/StaticDelegate.hpp>
#include <functional>
using namespace NGIN::Util;

namespace
{
    int SimpleFunction(int x, int y)
    {
        return x + y;
    }

    struct CallableObject
    {
        int operator()(int x, int y) const
        {
            return x * y;
        }
    };

    struct TestClass
    {
        int Multiply(int x, int y)
        {
            return x * y;
        }
    };
}

class StaticDelegateTest : public ::testing::Test
{
protected:
    // You can set up context here if needed
};

/// \brief Test if simple function can be wrapped.
TEST_F(StaticDelegateTest, SimpleFunctionTest)
{
    StaticDelegate<int(int, int)> del(SimpleFunction);
    EXPECT_EQ(del(2, 3), 5);
}

/// \brief Test if callable objects can be wrapped.
TEST_F(StaticDelegateTest, CallableObjectTest)
{
    CallableObject obj;
    StaticDelegate<int(int, int)> del(obj);
    EXPECT_EQ(del(2, 3), 6);
}

/// \brief Test if lambda functions can be wrapped.
TEST_F(StaticDelegateTest, LambdaTest)
{
    auto lambda = [](int x, int y) -> int
    { return x - y; };
    StaticDelegate<int(int, int)> del(lambda);
    EXPECT_EQ(del(5, 2), 3);
}

/// \brief Test if member functions can be wrapped.
TEST_F(StaticDelegateTest, MemberFunctionTest)
{
    TestClass obj;
    StaticDelegate<int(int, int)> del(&TestClass::Multiply, &obj);
    EXPECT_EQ(del(2, 3), 6);
}

/// \brief Test if the delegate can be invoked with arguments.
TEST_F(StaticDelegateTest, ArgumentInvokeTest)
{
    StaticDelegate<int(int, int)> del(SimpleFunction);
    int x = 5;
    int y = 2;
    EXPECT_EQ(del(x, y), 7);
}

// Add more tests based on what you need to cover
