/*
 * FOSOS memory management (malloc, free)
 * Found some of these concepts in a very old version of SerenityOS :~)
 *
 * Copyright (c) 2022, the FOSOS developers
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mem.h>

volatile UInt32 total_allocated;
volatile UInt32 total_free;

static byte map[MAP_SIZE];

void out_of_memory() {
	print("KERNEL PANIC - OUT OF MEMORY");
	hang();
}

void* malloc(UInt32 amount) {
	UInt32 total_amount = amount + sizeof(Allocation);

	// How did this happen?
	if(total_free < total_amount) out_of_memory();

	// Find out how many chunks we need
	UInt32 chunks = total_amount / HEAP_BLOCK;
	if(total_amount % HEAP_BLOCK) chunks++;

	UInt32 first_chunk = 0;
	UInt32 chunks_here = 0;

	// Go through each map node
	for(int i = 0; i < MAP_SIZE; i++) {
		for(int j = 0; j < 8; j++) {
			if(!(map[i] & (1 << j))) {
				// Get the first chunk
				if(chunks_here == 0) first_chunk = i * 8 + j;
				chunks_here++;

				// Done?
				if(chunks_here == chunks) {
					// Set the allocation's attributes
					Allocation* allocation = (Allocation*)(HEAP_BASE + (first_chunk * HEAP_BLOCK));
					byte* ptr = (byte*)allocation;
					ptr += sizeof(Allocation);
					allocation->amount = chunks;
					allocation->start = first_chunk;

					// Mark as used
					for(UInt32 k = first_chunk; k < (first_chunk + chunks); k++) map[k / 8] |= 1 << (k % 8);

					total_allocated += allocation->amount * HEAP_BLOCK;
					total_free -= allocation->amount * HEAP_BLOCK;

					print("malloc: %u used, %u free\n", total_allocated, total_free);

					return ptr;
				}
			} else chunks_here = 0;
		}
	}

	// Oh no!
	out_of_memory();

	// Prevent the compiler from complaining we don't return (we won't)
	return nullptr;
}

void free(void* ptr) {
	// Ignore if no pointer
	if(!ptr) return;

	Allocation* allocation = (Allocation*)((((byte*)ptr) - sizeof(Allocation)));

	// Mark as free! :~)
	for(UInt32 k = allocation->start; k < (allocation->start + allocation->amount); k++) map[k / 8] &= ~(1 << (k % 8));

	total_allocated -= allocation->amount * HEAP_BLOCK;
	total_free += allocation->amount * HEAP_BLOCK;

	print("free: %u used, %u free\n", total_allocated, total_free);
}

void mem_init() {
	memset(&map, 0, sizeof(map));
	total_allocated = 0;
	total_free = HEAP_SIZE;
}