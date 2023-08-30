#include <gtest/gtest.h>
#include <NGIN/TypeErasure/HybridStoragePolicy.hpp> // Replace with the actual path

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
class HybridStoragePolicyTest : public ::testing::Test
{
};

TEST_F(HybridStoragePolicyTest, HandlesSmallTypeByValue)
{

    TypeErasure::HybridStoragePolicy<128> policy{SmallType()};

    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(HybridStoragePolicyTest, HandlesLargeTypeByValue)
{
    TypeErasure::HybridStoragePolicy<128> policy{LargeType()};
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(HybridStoragePolicyTest, HandlesSmallTypeByMove)
{
    SmallType small;
    TypeErasure::HybridStoragePolicy<128> policy(std::move(small));
    SmallType *ptr = static_cast<SmallType *>(policy.get());
    EXPECT_EQ(ptr->x, 42);
}

TEST_F(HybridStoragePolicyTest, HandlesLargeTypeByMove)
{
    LargeType large;
    TypeErasure::HybridStoragePolicy<128> policy(std::move(large));
    LargeType *ptr = static_cast<LargeType *>(policy.get());
    EXPECT_EQ(ptr->data[0], 'a');
}

TEST_F(HybridStoragePolicyTest, DestructorForSmallType)
{
    {
        TypeErasure::HybridStoragePolicy<128> policy{SmallType()};
        SmallType *ptr = static_cast<SmallType *>(policy.get());
        EXPECT_EQ(ptr->x, 42);
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

TEST_F(HybridStoragePolicyTest, DestructorForLargeType)
{

    {
        TypeErasure::HybridStoragePolicy<128> policy{LargeType()};
        LargeType *ptr = static_cast<LargeType *>(policy.get());
        EXPECT_EQ(ptr->data[0], 'a');
    }
    // Here, the destructor for `policy` will have been called.
    EXPECT_EQ(destroyed, true);
}

// If you wish to test for memory leaks, you might have to use additional tools or libraries.
