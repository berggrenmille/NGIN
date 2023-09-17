#include <gtest/gtest.h>
#include <NGIN/Meta/StoragePolicy/HybridStorage.hpp> // Replace with the actual path

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

        LargeType(LargeType &&other) noexcept
        {
            std::memcpy(data, other.data, 200);
            other.data[0] = 0;
        }
        LargeType &operator=(LargeType &&other)
        {
            std::memcpy(data, other.data, 200);
            other.data[0] = 0;
            return *this;
        }
    };

    struct SmallType
    {
        int x;
        SmallType() : x(42) { destroyed = false; }
        ~SmallType() { destroyed = true; }

        SmallType(SmallType &&other) noexcept : x(other.x) { other.x = 0; }
        SmallType &operator=(SmallType &&other) = default;
    };
}
class HybridStorageTest : public ::testing::Test
{
};

TEST_F(HybridStorageTest, HandlesSmallTypeByValue)
{

    Meta::StoragePolicy::HybridStorage<128> policy{SmallType()};

    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(HybridStorageTest, HandlesLargeTypeByValue)
{
    Meta::StoragePolicy::HybridStorage<128> policy{LargeType()};
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(HybridStorageTest, HandlesSmallTypeByMove)
{
    SmallType small;
    Meta::StoragePolicy::HybridStorage<128> policy(std::move(small));
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(HybridStorageTest, HandlesLargeTypeByMove)
{
    LargeType large;
    Meta::StoragePolicy::HybridStorage<128> policy(std::move(large));
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(HybridStorageTest, DestructorForSmallType)
{
    {
        Meta::StoragePolicy::HybridStorage<128> policy{SmallType()};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStorageTest, DestructorForLargeType)
{

    {
        Meta::StoragePolicy::HybridStorage<128> policy{LargeType()};
        LargeType *ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStorageTest, MoveConstructorForSmallType)
{
    {
        Meta::StoragePolicy::HybridStorage<128> policy{SmallType()};
        Meta::StoragePolicy::HybridStorage<128> policy2(std::move(policy));
        SmallType *ptr = static_cast<SmallType *>(policy2.get());
        EXPECT_EQ(ptr->x, 42);
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStorageTest, MoveConstructorForLargeType)
{
    {
        Meta::StoragePolicy::HybridStorage<128> policy{LargeType()};
        Meta::StoragePolicy::HybridStorage<128> policy2(std::move(policy));
        LargeType *ptr = static_cast<LargeType *>(policy2.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

// If you wish to test for memory leaks, you might have to use additional tools or libraries.
