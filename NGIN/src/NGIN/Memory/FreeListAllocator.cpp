#include <PCH.h>
#include <NGIN/Memory/FreeListAllocator.hpp>
#include <NGIN/Memory/Internal/Alignment.hpp>

#include <cassert>
#include <NGIN/Logging.hpp>

namespace NGIN::Memory
{
	using Adress = uintptr_t;
	FreeListAllocator::FreeListAllocator(size_t size)
		: freeBlocks(nullptr), start(nullptr)
	{
		NGIN_ASSERT(size > sizeof(FreeBlock), "Size should be greater than the size of a FreeBlock: {} bytes", sizeof(FreeBlock));

		this->size = size;
		this->usedMemory = 0;
		start = ::operator new(size);
		freeBlocks = static_cast<FreeBlock *>(start);
		// Might be able to handle this error more gracefully, like throwing an exception or falling back to another allocator
		// Very unlikely, soooo... TODO
		NGIN_ASSERT(freeBlocks != nullptr, "FreeListAllocator failed to allocate memory at contruction");

		freeBlocks->size = size;
		freeBlocks->previous = nullptr;
		freeBlocks->next = nullptr;
	}

	FreeListAllocator::~FreeListAllocator()
	{
		::operator delete(start);
	}

	FreeListAllocator::FreeListAllocator(FreeListAllocator &&other) noexcept
		: start(other.start),
		  size(other.size),
		  usedMemory(other.usedMemory),
		  freeBlocks(other.freeBlocks)
	{
		// Set other's members to default state to ensure it doesn't deallocate memory on destruction.
		other.start = nullptr;
		other.size = 0;
		other.usedMemory = 0;
		other.freeBlocks = nullptr;
	}

	void *FreeListAllocator::Allocate(size_t size, size_t alignment, const std::source_location &location)
	{
		NGIN_ASSERT(size > 0, "Cannot allocate zero bytes.");

		// Find a suitable free block for the allocation.
		auto [foundBlock, headerAdjustment] = FindFreeBlock(size, alignment);
		// If no suitable block is found, log the error and return nullptr.
		if (foundBlock == nullptr) [[unlikely]]
		{
			NGIN_LOG_SRC(location, Logging::Level::Error, "Allocator reached it's capacity\n NOTE: Some engine provided allocators supports configurable size in Config.json");
			return nullptr;
		}

		// Remove the found block from the free list.
		RemoveBlockFromFreeList(foundBlock);
		// Store a copy of the old block's metadata.
		FreeBlock oldBlock = *foundBlock;

		// Calculate the locations for the AllocationHeader and the payload.
		uintptr_t headerPtr = uintptr_t(foundBlock) + headerAdjustment;
		uintptr_t payloadPtr = headerPtr + sizeof(AllocationHeader);

		// Determine the available space for the payload.
		size_t payloadSpace = oldBlock.size - sizeof(AllocationHeader) - headerAdjustment;
		auto header = new (reinterpret_cast<void *>(headerPtr)) AllocationHeader(payloadSpace, headerAdjustment);

		uintptr_t payloadEnd = payloadPtr + size;
		uintptr_t blockEnd = payloadPtr + payloadSpace;
		// Check if a new free block can be created from the remaining space.

		size_t newBlockAdjustment = Internal::GetAlignmentPadding(reinterpret_cast<void *>(payloadEnd), alignof(FreeBlock));
		size_t padding = blockEnd - (payloadEnd + newBlockAdjustment);

		if (padding >= minBlockSize)
		{
			header->size -= padding;

			void *newBlockPtr = reinterpret_cast<void *>(payloadEnd + newBlockAdjustment);
			FreeBlock *newBlock = new (newBlockPtr) FreeBlock(padding);

			AddBlockToFreeList(newBlock);
		}

		usedMemory += header->size;
		return reinterpret_cast<void *>(payloadPtr);
	}

	void FreeListAllocator::Deallocate(void *ptr)
	{
		NGIN_ASSERT(ptr != nullptr, "Cannot deallocate a nullptr.");

		AllocationHeader *header = (AllocationHeader *)((uintptr_t)ptr - sizeof(AllocationHeader));
		size_t blockSize = header->adjustment + header->size + sizeof(AllocationHeader);

		void *block = (void *)((uintptr_t)ptr - header->adjustment);

		usedMemory -= header->size;

		FreeBlock *newBlock = new (block) FreeBlock(blockSize);

		AddBlockToFreeList(newBlock);
	}

	void FreeListAllocator::DeallocateAll()
	{
		freeBlocks = reinterpret_cast<FreeBlock *>(new char[size]);
		freeBlocks->size = size;
		freeBlocks->next = nullptr;

		usedMemory = 0;
	}

	bool FreeListAllocator::Owns(void *ptr) const
	{
		// Convert the void pointers to uintptr_t for arithmetic
		uintptr_t startAddress = reinterpret_cast<uintptr_t>(start);
		uintptr_t endAddress = startAddress + size;
		uintptr_t targetAddress = reinterpret_cast<uintptr_t>(ptr);

		// Check if the targetAddress is within the range [startAddress, endAddress)
		return targetAddress >= startAddress && targetAddress < endAddress;
	}

	void FreeListAllocator::AddBlockToFreeList(FreeBlock *block)
	{

		if (freeBlocks == nullptr)
		{
			freeBlocks = block;
			return;
		}

		if ((uintptr_t)block < (uintptr_t)freeBlocks)
		{
			block->next = freeBlocks;
			freeBlocks->previous = block;
			freeBlocks = block;
			CoalesceBlock(block);
			return;
		}

		auto currentBlock = freeBlocks;

		while (currentBlock->next != nullptr)
		{
			if ((uintptr_t)block > (uintptr_t)currentBlock &&
				(uintptr_t)block < (uintptr_t)(currentBlock->next))
			{

				block->next = currentBlock->next;
				currentBlock->next->previous = block;
				block->previous = currentBlock;
				currentBlock->next = block;
				CoalesceBlock(block);
				return;
			}
			currentBlock = currentBlock->next;
		}

		currentBlock->next = block;
		block->previous = currentBlock;
		CoalesceBlock(block);
	}

	void FreeListAllocator::RemoveBlockFromFreeList(FreeBlock *block)
	{
		NGIN_ASSERT(block != nullptr, "Cannot remove null block");

		if (block == freeBlocks)
		{
			freeBlocks = block->next;
			return;
		}

		if (block->previous != nullptr)
			block->next->previous = block->previous;

		if (block->next != nullptr)
			block->next->previous = block->previous;
	}

	void FreeListAllocator::CoalesceBlock(FreeBlock *block)
	{
		if (block->previous != nullptr)
		{
			uintptr_t leftEnd = uintptr_t(block->previous) + block->previous->size;
			if (uintptr_t(block) == leftEnd)
			{
				block->previous->next = block->next;
				if (block->next != nullptr)
					block->next->previous = block->previous;
				block->previous->size += block->size;
				block = block->previous;
			}
		}

		if (block->next != nullptr)
		{
			uintptr_t blockEnd = uintptr_t(block) + block->size;

			// If the end of this block is the start of the next block, coalesce
			if (blockEnd == uintptr_t(block->next))
			{
				block->size += block->next->size;
				block->next = block->next->next;
				if (block->next != nullptr)
				{
					block->next->previous = block;
				}
			}
		}
	}

	std::pair<FreeListAllocator::FreeBlock *, size_t> FreeListAllocator::FindFreeBlock(size_t size, size_t alignment)
	{
		const auto totSize = size + sizeof(AllocationHeader);

		const size_t reqAlignment = std::max(alignof(AllocationHeader), alignment);

		FreeBlock *currentBlock = freeBlocks;

		size_t headerAdjustment = 0;

		while (currentBlock != nullptr)
		{
			uintptr_t first = reinterpret_cast<uintptr_t>(currentBlock) + sizeof(AllocationHeader);

			size_t reqAdjustment =
				Internal::GetAlignmentPadding(reinterpret_cast<void *>(first), reqAlignment);
			size_t totAllocSize = totSize + reqAdjustment;

			if (totAllocSize <= currentBlock->size)
			{
				return {currentBlock, reqAdjustment};
			}

			currentBlock = currentBlock->next;
		}
		return {nullptr, headerAdjustment};
	}

} // namespace NGIN
