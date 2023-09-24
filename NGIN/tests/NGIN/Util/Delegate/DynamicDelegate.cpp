#include <gtest/gtest.h>
#include <NGIN/Util/Delegate/DynamicDelegate.hpp>
#include <functional>
using namespace NGIN::Util;

namespace
{
    int Sum(int x, int y)
    {
        return x + y;
    }

    void Noop()
    {
        return;
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

        int ConstMultiply(int x, int y) const
        {
            return x * y;
        }
    };
}

class DynamicDelegateTest : public ::testing::Test
{
protected:
    // You can set up context here if needed
};

/// \brief Test if free functions can be wrapped.
TEST_F(DynamicDelegateTest, FreeFunctionTest)
{
    DynamicDelegate del(Sum);
    EXPECT_EQ(del.Return<int>(2, 3), 5);
}

/// \brief Test if no-op functions can be wrapped.
TEST_F(DynamicDelegateTest, NoopTest)
{
    DynamicDelegate del(Noop);
    del();
    SUCCEED();
}

/// \brief Test if callable objects can be wrapped.
TEST_F(DynamicDelegateTest, CallableObjectTest)
{
    CallableObject obj;
    DynamicDelegate del(obj);
    EXPECT_EQ(del.Return<int>(2, 3), 6);
}

/// \brief Test if member functions can be wrapped.
TEST_F(DynamicDelegateTest, MemberFunctionTest)
{
    TestClass obj;
    DynamicDelegate del(&TestClass::Multiply, &obj);
    EXPECT_EQ(del.Return<int>(2, 3), 6);
}

/// \brief Test if const member functions can be wrapped.
TEST_F(DynamicDelegateTest, ConstMemberFunctionTest)
{
    TestClass obj;
    DynamicDelegate del(&TestClass::ConstMultiply, &obj);
    EXPECT_EQ(del.Return<int>(2, 3), 6);
}

/// \brief Test for return type mismatch.
TEST_F(DynamicDelegateTest, ReturnTypeMismatchTest)
{
    DynamicDelegate del(Sum);
    ASSERT_ANY_THROW(del.Return<std::string>(2, 3));
}

/// \brief Test if capturing lambdas can be wrapped.
TEST_F(DynamicDelegateTest, CapturingLambdaTest)
{
    int a = 1;
    auto lambda = [a](int x) -> int
        { return x + a; };
    DynamicDelegate del(lambda);
    EXPECT_EQ(del.Return<int>(2), 3);
}