#include <gtest/gtest.h>
#include <NGIN/TypeErasure/StaticStoragePolicy.hpp> // Replace with the actual path

#include <iostream>

using namespace NGIN;
namespace
{
    bool destroyed = false;

    struct LargeType
    {
        char data[200];

        LargeType()
        {
            std::fill_n(data, 200, 'a');
            destroyed = false;
        }
        ~LargeType()
        {
            destroyed = true;
        }
    };

    struct SmallType
    {
        int x;
        SmallType() : x(42) { destroyed = false; }
        ~SmallType() { destroyed = true; }
    };

}

class StaticStoragePolicyTest : public ::testing::Test
{
};

TEST_F(StaticStoragePolicyTest, HandlesSmallTypeByValue)
{
    TypeErasure::StaticStoragePolicy<128> policy{SmallType()};
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(StaticStoragePolicyTest, HandlesSmallTypeByMove)
{
    SmallType small;
    TypeErasure::StaticStoragePolicy<128> policy(std::move(small));
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(StaticStoragePolicyTest, DestructorForSmallType)
{
    {
        TypeErasure::StaticStoragePolicy<128> policy{SmallType()};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

// Compilation failure test: Uncomment the test below to check if static_assert works
/*
TEST_F(StaticStoragePolicyTest, CompileTimeCheckForSize)
{
    LargeType *ptr;
    {
        TypeErasure::StaticStoragePolicy<128> policy{LargeType()};
        ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);

}
*/
