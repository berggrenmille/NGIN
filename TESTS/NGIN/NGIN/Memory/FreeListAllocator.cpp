#include <gtest/gtest.h>
#include <NGIN/Memory/FreeListAllocator.hpp>

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
	void* block1 = allocator.Allocate(128);
	EXPECT_NE(block1, nullptr);

	void* block2 = allocator.Allocate(128);
	EXPECT_NE(block2, nullptr);
}

TEST_F(FreeListAllocatorTest, OverAllocation)
{
	void* block1 = allocator.Allocate(256);
	EXPECT_NE(block1, nullptr);

	void* block2 = allocator.Allocate(256);
	EXPECT_NE(block2, nullptr);

	void* block3 = allocator.Allocate(256);
	EXPECT_NE(block3, nullptr);

	void* block4 = allocator.Allocate(256);
	EXPECT_EQ(block4, nullptr);  // Should be out of memory after previous allocations
}

TEST_F(FreeListAllocatorTest, Alignment)
{
	void* block = allocator.Allocate(1, 256);  // 256 byte alignment
	uintptr_t address = reinterpret_cast<uintptr_t>(block);
	EXPECT_EQ(address % 256, 0);  // Check if the address is aligned
}

TEST_F(FreeListAllocatorTest, DeallocateAndReallocate)
{
	void* block1 = allocator.Allocate(512);
	allocator.Deallocate(block1);
	void* block2 = allocator.Allocate(512);
	EXPECT_NE(block2, nullptr);  // We should be able to allocate the same size block after deallocating
}

TEST_F(FreeListAllocatorTest, DeallocateAll)
{
	allocator.Allocate(512);
	allocator.DeallocateAll();

	void* block = allocator.Allocate(512);
	EXPECT_NE(block, nullptr);  // We should be able to allocate the entire block after a deallocate all
}

TEST_F(FreeListAllocatorTest, UsedMemory)
{
	allocator.Allocate(200);
	size_t used = allocator.getUsedMemory();
	EXPECT_EQ(used, 200);
}

TEST_F(FreeListAllocatorTest, Fragmentation)
{
	void* block1 = allocator.Allocate(200);
	void* block2 = allocator.Allocate(200);
	allocator.Deallocate(block2); // Free the second 200 bytes
	allocator.Deallocate(block1); // Free the first 200 bytes

	void* block3 = allocator.Allocate(300);  // Should allocate in the space after block2 due to fragmentation
	EXPECT_NE(block3, nullptr);
}


