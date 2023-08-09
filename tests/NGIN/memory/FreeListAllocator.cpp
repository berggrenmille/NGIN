#include <gtest/gtest.h>
#include <Memory/FreeListAllocator.h>

using namespace NGIN;

class FreeListAllocatorTest : public ::testing::Test
{
protected:
	FreeListAllocator allocator;

	FreeListAllocatorTest() : allocator(1024) {}  // Initializes a block of 1024 bytes for each test
};

TEST_F(FreeListAllocatorTest, BasicAllocation)
{
	void* block = allocator.Allocate(512);
	EXPECT_NE(block, nullptr);
}

TEST_F(FreeListAllocatorTest, MultipleAllocations)
{
	void* block1 = allocator.Allocate(300);
	EXPECT_NE(block1, nullptr);

	void* block2 = allocator.Allocate(300);
	EXPECT_NE(block2, nullptr);

	void* block3 = allocator.Allocate(300);
	EXPECT_EQ(block3, nullptr);  // Should be out of memory after previous allocations
}

TEST_F(FreeListAllocatorTest, DeallocationAndMerging)
{
	void* block1 = allocator.Allocate(300);
	void* block2 = allocator.Allocate(300);
	allocator.Deallocate(block2);
	void* block3 = allocator.Allocate(500);  // We should be able to allocate 500 bytes after deallocating block2
	EXPECT_NE(block3, nullptr);
}

TEST_F(FreeListAllocatorTest, DeallocationAll)
{
	allocator.Allocate(300);
	allocator.DeallocateAll();

	void* block = allocator.Allocate(1024);
	EXPECT_NE(block, nullptr);  // We should be able to allocate the entire block after a deallocate all
}

TEST_F(FreeListAllocatorTest, RemainingMemory)
{
	allocator.Allocate(300);
	allocator.Allocate(300);
	size_t used = allocator.getUsedMemory();  // Assuming you've this function similar to LinearAllocator's getRemainingMemory
	EXPECT_EQ(used, 600);  // 300 + 300 = 600 bytes used
}
