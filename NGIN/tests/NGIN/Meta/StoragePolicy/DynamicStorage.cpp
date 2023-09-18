#include <gtest/gtest.h>
#include <NGIN/Meta/StoragePolicy/DynamicStorage.hpp>
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
class DynamicStorageTest : public ::testing::Test
{
};

TEST_F(DynamicStorageTest, HandlesSmallTypeByValue)
{
    NGIN::Meta::StoragePolicy::DynamicStorage policy{SmallType()};
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(DynamicStorageTest, HandlesSmallTypeByMove)
{
    SmallType small;
    NGIN::Meta::StoragePolicy::DynamicStorage policy(std::move(small));
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(DynamicStorageTest, DestructorForSmallType)
{
    {
        NGIN::Meta::StoragePolicy::DynamicStorage policy{SmallType()};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    EXPECT_EQ(destroyed, true);
}

TEST_F(DynamicStorageTest, HandlesLargeTypeByValue)
{
    NGIN::Meta::StoragePolicy::DynamicStorage policy{LargeType()};
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(DynamicStorageTest, HandlesLargeTypeByMove)
{
    LargeType large;
    NGIN::Meta::StoragePolicy::DynamicStorage policy(std::move(large));
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(DynamicStorageTest, DestructorForLargeType)
{
    {
        NGIN::Meta::StoragePolicy::DynamicStorage policy{LargeType()};
        LargeType *ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    EXPECT_EQ(destroyed, true);
}
