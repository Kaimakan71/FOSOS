/**
 * FOSOS memory manager
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <memory.h>

#define HEAP_START 0x2100 // After video mode info and stack
#define HEAP_BLOCK 128    // 128-byte chunks
#define HEAP_SIZE  57088  // 55.75KB
#define MAP_SIZE   (HEAP_SIZE / HEAP_BLOCK / 8)

typedef struct {
	UInt32 start;
	UInt32 count;
} PACKED Allocation;

static byte map[MAP_SIZE];

uint total_allocated;
uint total_free;

#define heapFull() debugf("Cannot malloc: out of memory (heap size %u)\n", HEAP_SIZE); hang();

void* malloc(uint amount) {
	// We have to put an Allocation at the start of the block(s)
	uint realSize = amount = sizeof(Allocation);

	if(total_free < realSize) {
		heapFull();
		return nullptr;
	}

	// Figure out how many chunks we need, rounding up
	uint chunksNeeded = realSize / HEAP_BLOCK;
	if(realSize % HEAP_BLOCK) chunksNeeded++;

	uint chunksPresent = 0;
	uint firstChunk = 0;

	// Allocation time!
	for(uint i = 0; i < MAP_SIZE; i++) {
		if(map[i] == 0xff) {
			chunksPresent = 0;
			continue;
		}

		for(uint j = 0; j < 8; j++) {
			if(!(map[i] & (1 << j))) {
				if(chunksPresent == 0) firstChunk = i * 8 + j;

				chunksPresent++;

				if(chunksPresent == chunksNeeded) {
					Allocation* alloc = (Allocation*)(HEAP_START + (firstChunk * HEAP_BLOCK));
					byte* ptr = (byte*)alloc;
					ptr += sizeof(Allocation);
					alloc->start = firstChunk;
					alloc->count = chunksPresent;

					// Mark as used
					for(uint k = firstChunk; k < (firstChunk + chunksPresent); k++) {
						map[k / 8] |= 1 << (k % 8);
					}

					uint bytes = alloc->count * HEAP_BLOCK;
					total_allocated += bytes;
					total_free -= bytes;

					// Zero the entire allocation to protect from memory randomization or previous allocation contents
					memset(ptr, 0, bytes - sizeof(Allocation));

					debugf("Malloc. (%u/%u)\n", total_allocated, HEAP_SIZE);

					return ptr;
				}
			} else chunksPresent = 0;
		}
	}

	heapFull();
	return nullptr;
}

void free(void* ptr) {
	if(!ptr) return;

	Allocation* alloc = (Allocation*)((((byte*)ptr) - sizeof(Allocation)));

	for(uint k = alloc->start; k < (alloc->start + alloc->count); k++) {
		// Mark as unused
		map[k / 8] &= ~(1 << (k % 8));
	}

	uint bytes = alloc->count * HEAP_BLOCK;
	total_allocated -= bytes;
	total_free += bytes;

	debugf("Free! (%u/%u)\n", total_allocated, HEAP_SIZE);
}

void memory_init() {
	// Clear out the allocation map
	memset(&map, 0, sizeof(map));

	total_allocated = 0;
	total_free = HEAP_SIZE;

	debugf("Memory manager initialized with a heap size of %uKB\n", HEAP_SIZE / 1024);
}