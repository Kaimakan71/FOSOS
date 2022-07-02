/*
 * FOSOS memory management (malloc, free)
 *
 * Copyright (c) 2022, the FOSOS developers
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <stdlib.h>
#include <assert.h>
#include <vga.h>

#define HEAP_BLOCK 128
#define HEAP_SIZE (62 * 1024)
#define HEAP_BASE 0x2000
#define MAP_SIZE (HEAP_SIZE / HEAP_BLOCK / 8)

typedef struct {
	UInt32 start;
	UInt32 amount;
} PACKED Allocation;

extern volatile UInt32 total_allocated;
extern volatile UInt32 total_free;

void* MALLOC kmalloc(UInt32 amount) MALLOC;
void kfree(void*);
void mem_init();