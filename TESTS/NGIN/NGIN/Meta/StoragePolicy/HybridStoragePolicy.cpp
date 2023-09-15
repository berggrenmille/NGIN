#include <gtest/gtest.h>
#include <NGIN/Meta/StoragePolicy/Hybrid.hpp> // Replace with the actual path

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

        void Move(LargeType &&other) noexcept
        {
            std::memcpy(data, other.data, 200);
            other.data[0] = 0;
        }
    };

    struct SmallType
    {
        int x;
        SmallType() : x(42) { destroyed = false; }
        ~SmallType() { destroyed = true; }

        SmallType(SmallType &&other) noexcept : x(other.x) { other.x = 0; }
        SmallType &operator=(SmallType &&other) = default;
        void Move(SmallType &&other) noexcept { x = other.x; }
    };
}
class HybridStoragePolicyTest : public ::testing::Test
{
};

TEST_F(HybridStoragePolicyTest, HandlesSmallTypeByValue)
{

    Meta::StoragePolicy::Hybrid<128> policy{SmallType()};

    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(HybridStoragePolicyTest, HandlesLargeTypeByValue)
{
    Meta::StoragePolicy::Hybrid<128> policy{LargeType()};
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(HybridStoragePolicyTest, HandlesSmallTypeByMove)
{
    SmallType small;
    Meta::StoragePolicy::Hybrid<128> policy(std::move(small));
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(HybridStoragePolicyTest, HandlesLargeTypeByMove)
{
    LargeType large;
    Meta::StoragePolicy::Hybrid<128> policy(std::move(large));
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(HybridStoragePolicyTest, DestructorForSmallType)
{
    {
        Meta::StoragePolicy::Hybrid<128> policy{SmallType()};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStoragePolicyTest, DestructorForLargeType)
{

    {
        Meta::StoragePolicy::Hybrid<128> policy{LargeType()};
        LargeType *ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStoragePolicyTest, MoveConstructorForSmallType)
{
    {
        Meta::StoragePolicy::Hybrid<128> policy{SmallType()};
        Meta::StoragePolicy::Hybrid<128> policy2(std::move(policy));
        SmallType *ptr = static_cast<SmallType *>(policy2.get());
        EXPECT_EQ(ptr->x, 42);
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStoragePolicyTest, MoveConstructorForLargeType)
{
    {
        Meta::StoragePolicy::Hybrid<128> policy{LargeType()};
        Meta::StoragePolicy::Hybrid<128> policy2(std::move(policy));
        LargeType *ptr = static_cast<LargeType *>(policy2.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

// If you wish to test for memory leaks, you might have to use additional tools or libraries.
