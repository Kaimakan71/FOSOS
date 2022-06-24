/*
 * FOSOS memory management (malloc, free)
 *
 * Copyright (c) 2022, the FOSOS developers
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <stdlib.h>

#define HEAP_BLOCK 128
#define HEAP_SIZE 491520 // 480KiB
#define HEAP_BASE 0x8000 // Start at physical 0x8000
#define MAP_SIZE (HEAP_SIZE / HEAP_BLOCK / 8)

typedef struct {
	UInt32 start;
	UInt32 amount;
} PACKED Allocation;

extern volatile UInt32 total_allocated;
extern volatile UInt32 total_free;

void* malloc(UInt32 amount) MALLOC;
void free(void*);
void mem_init();