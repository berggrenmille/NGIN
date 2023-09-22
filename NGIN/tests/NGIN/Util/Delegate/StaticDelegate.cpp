#include <gtest/gtest.h>
#include <NGIN/Util/Delegate/StaticDelegate.hpp>
#include <functional>
using namespace NGIN::Util;

namespace
{
    int ReturningSimpleFunction(int x, int y)
    {
        return x + y;
    }

    void VoidSimpleFunction()
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

class StaticDelegateTest : public ::testing::Test
{
protected:
    // You can set up context here if needed
};

TEST_F(StaticDelegateTest, VoidFunctionTest)
{
    StaticDelegate<void()> del(VoidSimpleFunction);
    del();
    SUCCEED();
}

TEST_F(StaticDelegateTest, VoidLambdaTest)
{
    auto lambda = []() -> void
    { return; };
    StaticDelegate<void()> del(lambda);
    del();
    SUCCEED();
}

TEST_F(StaticDelegateTest, LValueArgumentTest)
{
    auto lambda = [](int &x) -> void
    { x = 5; };
    StaticDelegate<void(int &)> del(lambda);
    int x = 0;
    del(x);
    EXPECT_EQ(x, 5);
}

TEST_F(StaticDelegateTest, RValueArgumentTest)
{
    auto lambda = [](int &&x) -> void
    { return; };
    StaticDelegate<void(int &&)> del(lambda);
    del(5);
    SUCCEED();
}

TEST_F(StaticDelegateTest, ConstLValueArgumentTest)
{
    auto lambda = [](const int &x) -> void
    { return; };
    StaticDelegate<void(const int &)> del(lambda);
    int x = 0;
    del(x);
    SUCCEED();
}

TEST_F(StaticDelegateTest, ConstRValueArgumentTest)
{
    auto lambda = [](const int &&x) -> void
    { return; };
    StaticDelegate<void(const int &&)> del(lambda);
    del(5);
    SUCCEED();
}

/// \brief Test if simple function can be wrapped.
TEST_F(StaticDelegateTest, SimpleFunctionTest)
{
    StaticDelegate<int(int, int)> del(ReturningSimpleFunction);
    EXPECT_EQ(del(2, 3), 5);
}

/// \brief Test if callable objects can be wrapped.
TEST_F(StaticDelegateTest, CallableObjectTest)
{
    CallableObject obj;
    StaticDelegate<int(int, int)> del(obj);
    EXPECT_EQ(del(2, 3), 6);
}

/// \brief Test if member functions can be wrapped.
TEST_F(StaticDelegateTest, MemberFunctionTest)
{
    TestClass obj;
    StaticDelegate<int(int, int)> del(&TestClass::Multiply, &obj);
    EXPECT_EQ(del(2, 3), 6);
}

TEST_F(StaticDelegateTest, ConstMemberFunctionTest)
{
    TestClass obj;
    StaticDelegate<int(int, int)> del(&TestClass::ConstMultiply, &obj);
    EXPECT_EQ(del(2, 3), 6);
}

TEST_F(StaticDelegateTest, NullVoidFunctionPointerTest)
{
    ASSERT_ANY_THROW(StaticDelegate<void()> del(nullptr));
}

TEST_F(StaticDelegateTest, NullReturningFunctionPointerTest)
{
    ASSERT_ANY_THROW(StaticDelegate<int(int, int)> del(nullptr));
}

TEST_F(StaticDelegateTest, MultipleInstancesTest)
{
    StaticDelegate<int(int, int)> del1(ReturningSimpleFunction);
    StaticDelegate<int(int, int)> del2(ReturningSimpleFunction);
    EXPECT_EQ(del1(2, 3), 5);
    EXPECT_EQ(del2(2, 3), 5);
}

TEST_F(StaticDelegateTest, StatefulLambda)
{
    int a = 1;
    auto lambda = [a](int x) -> int
    { return x + a; };
    StaticDelegate<int(int)> del(lambda);
    EXPECT_EQ(del(2), 3);
}

TEST_F(StaticDelegateTest, ChainedDelegates)
{
    StaticDelegate<int(int, int)> del1(ReturningSimpleFunction);
    StaticDelegate<int(int, int)> del2([&del1](int x, int y)
                                       { return del1(x, y); });
    EXPECT_EQ(del2(2, 3), 5);
}

TEST_F(StaticDelegateTest, NonCopyableCallable)
{
    struct NonCopyable
    {
        NonCopyable() = default;
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable(NonCopyable &&) = default;
        int operator()(int x) { return x + 1; }
    };

    NonCopyable obj;
    StaticDelegate<int(int)> del(std::move(obj));
    EXPECT_EQ(del(1), 2);
}
