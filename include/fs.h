/*
 * FOSOS File System
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <vga.h>
#include <ide.h>
#include <assert.h>

#define FAT_ENTRY_BYTES 4
#define BLOCK_BYTES 1024

namespace FS {

// 32 bytes
typedef struct {
	char name[24];
	int size;
	int firstBlock;
} PACKED DirEnt;

void init();

};