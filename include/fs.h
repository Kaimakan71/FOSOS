/*
 * FOSOS File System
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <mem.h>
#include <vga.h>
#include <ide.h>
#include <assert.h>

#define BLOCK_SIZE 1024
#define SECTOR_SIZE 512
typedef UInt32 FATEntry;

namespace FS {

void init();

};