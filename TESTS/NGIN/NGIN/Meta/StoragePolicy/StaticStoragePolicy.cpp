
#include <gtest/gtest.h>
#include <NGIN/Meta/StoragePolicy/Static.hpp>
#include <cstddef>

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

		SmallType(SmallType&& other) noexcept : x(other.x) { other.x = 0; }
		SmallType& operator=(SmallType&& other) = default;
	};

	struct MovableType
	{
		int value;
		bool moved_from;

		MovableType(int v) : value(v), moved_from(false) {}
		MovableType(MovableType&& other) noexcept
			: value(other.value), moved_from(false)
		{
			other.moved_from = true;
		}
		MovableType& operator=(MovableType&& other) noexcept
		{
			value = other.value;
			moved_from = false;
			other.moved_from = true;
			return *this;
		}

	};

}

class StaticStoragePolicyTest : public ::testing::Test
{};

TEST_F(StaticStoragePolicyTest, HandlesSmallTypeByValue)
{
	Meta::StoragePolicy::Static<128> policy{SmallType()};
	SmallType* ptr = static_cast<SmallType*>(policy.get());
	EXPECT_EQ(ptr->x, 42);
}

TEST_F(StaticStoragePolicyTest, HandlesSmallTypeByMove)
{
	SmallType small;
	Meta::StoragePolicy::Static<128> policy(std::move(small));
	SmallType* ptr = static_cast<SmallType*>(policy.get());
	EXPECT_EQ(ptr->x, 42);
}

TEST_F(StaticStoragePolicyTest, DestructorForSmallType)
{
	{
		Meta::StoragePolicy::Static<128> policy{SmallType()};
		SmallType* ptr = static_cast<SmallType*>(policy.get());
		EXPECT_EQ(ptr->x, 42);
	}
	// Here, the destructor for `policy` will have been called.
	EXPECT_EQ(destroyed, true);
}

TEST_F(StaticStoragePolicyTest, HandlesMovableTypeMoveConstruct)
{
	MovableType mov(100);
	Meta::StoragePolicy::Static<128> policy{std::move(mov)};

	MovableType* ptr = static_cast<MovableType*>(policy.get());
	EXPECT_EQ(ptr->value, 100);
	EXPECT_EQ(mov.moved_from, true);
}

// Test move construction from another Static object
TEST_F(StaticStoragePolicyTest, MoveConstructFromAnotherStatic)
{
	Meta::StoragePolicy::Static<128> policy1{MovableType(200)};
	Meta::StoragePolicy::Static<128> policy2{std::move(policy1)};

	MovableType* ptr = static_cast<MovableType*>(policy2.get());
	EXPECT_EQ(ptr->value, 200);
}

// Test move assignment
TEST_F(StaticStoragePolicyTest, MoveAssignmentFromAnotherStatic)
{
	Meta::StoragePolicy::Static<128> policy1{MovableType(300)};
	Meta::StoragePolicy::Static<128> policy2;

	policy2 = std::move(policy1);

	MovableType* ptr = static_cast<MovableType*>(policy2.get());
	EXPECT_EQ(ptr->value, 300);
}

// Compilation failure test: Uncomment the test below to check if static_assert works
/*
TEST_F(StaticStoragePolicyTest, CompileTimeCheckForSize)
{
	LargeType *ptr;
	{
		Meta::StoragePolicy::Static<128> policy{LargeType()};
		ptr = static_cast<LargeType *>(policy.get());
		EXPECT_EQ(ptr->data[0], 'a');
	}
	// Here, the destructor for `policy` will have been called.
	EXPECT_EQ(destroyed, true);

}
*/
