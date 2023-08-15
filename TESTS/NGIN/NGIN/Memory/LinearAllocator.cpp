#include <gtest/gtest.h>
#include <NGIN/Memory/LinearAllocator.hpp>

using namespace NGIN;

class LinearAllocatorTest : public ::testing::Test
{
protected:
	LinearAllocator allocator;

	LinearAllocatorTest() : allocator(1024) {}  // Initializes a block of 1024 bytes for each test
};

TEST_F(LinearAllocatorTest, BasicAllocation)
{
	void* block = allocator.Allocate(512);
	EXPECT_NE(block, nullptr);
}

TEST_F(LinearAllocatorTest, OverAllocation)
{
	void* block1 = allocator.Allocate(512);
	EXPECT_NE(block1, nullptr);

	void* block2 = allocator.Allocate(512);
	EXPECT_NE(block2, nullptr);

	void* block3 = allocator.Allocate(1);
	EXPECT_EQ(block3, nullptr);  // Should be out of memory after previous allocations
}

TEST_F(LinearAllocatorTest, Alignment)
{
	void* block = allocator.Allocate(1, 256);  // 256 byte alignment
	uintptr_t address = reinterpret_cast<uintptr_t>(block);
	EXPECT_EQ(address % 256, 0);  // Check if the address is aligned
}

TEST_F(LinearAllocatorTest, DeallocateAll)
{
	allocator.Allocate(512);
	allocator.DeallocateAll();

	void* block = allocator.Allocate(1024);
	EXPECT_NE(block, nullptr);  // We should be able to allocate the entire block after a deallocate all
}

TEST_F(LinearAllocatorTest, RemainingMemory)
{
	allocator.Allocate(200);
	size_t remaining = allocator.getRemainingMemory();
	EXPECT_EQ(remaining, 824);  // 1024 - 200 = 824 bytes left
}

