#include <gtest/gtest.h>
#include <NGIN/Meta/DynamicStoragePolicy.hpp>
#include <utility>

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
class DynamicStoragePolicyTest : public ::testing::Test
{
};

TEST_F(DynamicStoragePolicyTest, HandlesSmallTypeByValue)
{
    NGIN::Meta::DynamicStoragePolicy policy{SmallType()};
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(DynamicStoragePolicyTest, HandlesSmallTypeByMove)
{
    SmallType small;
    NGIN::Meta::DynamicStoragePolicy policy(std::move(small));
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(DynamicStoragePolicyTest, DestructorForSmallType)
{
    {
        NGIN::Meta::DynamicStoragePolicy policy{SmallType()};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    EXPECT_EQ(destroyed, true);
}

TEST_F(DynamicStoragePolicyTest, HandlesLargeTypeByValue)
{
    NGIN::Meta::DynamicStoragePolicy policy{LargeType()};
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(DynamicStoragePolicyTest, HandlesLargeTypeByMove)
{
    LargeType large;
    NGIN::Meta::DynamicStoragePolicy policy(std::move(large));
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(DynamicStoragePolicyTest, DestructorForLargeType)
{
    {
        NGIN::Meta::DynamicStoragePolicy policy{LargeType()};
        LargeType *ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    EXPECT_EQ(destroyed, true);
}
