/*
 * FOSOS File System
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <fs.h>

namespace FS {

int block_sectors;
int disk_blocks;
int fat_entries;
int fat_bytes;
int fat_blocks;
int fat_entries_per_block;
int dir_entries_per_block;

int loaded_fat_block = -1; // Initially -1 so the FAT gets loaded by readBlocks
byte fat_block[BLOCK_BYTES];

// Read blocks from disk
void readRawBlocks(int start, int count, byte* buffer) {
	ASSERT(start + count < disk_blocks);
	print("Reading %u blocks from disk starting at %u\n", count, start);
	Disk::read((UInt32)(start * block_sectors), (UInt8)(count * block_sectors), buffer);
}

// Write blocks to disk
void writeRawBlocks(int start, int count, byte* buffer) {
	ASSERT(start + count < disk_blocks);
	print("Writing %u blocks to disk starting at %u\n", count, start);
	Disk::write((UInt32)(start * block_sectors), (UInt8)(count * block_sectors), buffer);
}

// Ensure that the loaded FAT block has the entry <entry>
void readFAT(int entry) {
	ASSERT(entry < fat_entries);

	int block = entry / fat_entries_per_block;
	print("Reading FAT entry %u from block %u\n", entry, block);

	// If we don't have the FAT block we need, get it
	if(loaded_fat_block != block) {
		print("Loading FAT from disk\n");
		readRawBlocks(block, 1, fat_block);
		loaded_fat_block = block;
	}
}

// Get an entry in the FAT, loading from disk if needed
int getFATEntry(int entry) {
	readFAT(entry);

	UInt32* table = (UInt32*)fat_block;
	return table[entry % fat_entries_per_block];
}

void setFATEntry(int entry, int value) {
	readFAT(entry);

	UInt32* table = (UInt32*)fat_block;
	table[entry % fat_entries_per_block] = value;

	// Write out to disk
	writeRawBlocks(loaded_fat_block, 1, fat_block);
}

void init() {
	print("Initializing file system :O\n");

	block_sectors = BLOCK_BYTES / 512;
	disk_blocks = DISK_BYTES / BLOCK_BYTES;

	fat_entries = disk_blocks;
	fat_bytes = fat_entries * FAT_ENTRY_BYTES;
	// Divide and round up: a/b = (a + (b - 1)) / b
	fat_blocks = (fat_bytes + (BLOCK_BYTES - 1)) / BLOCK_BYTES;
	fat_entries_per_block = BLOCK_BYTES / FAT_ENTRY_BYTES;
	dir_entries_per_block = BLOCK_BYTES / sizeof(DirEnt);

	print("fe=%u, fb=%u, fB=%u, fepB=%u depB=%u\n",
		fat_entries,
		fat_bytes,
		fat_blocks,
		fat_entries_per_block,
		dir_entries_per_block
	);

	// If you run initfs >1x, you will see 0 the first time and 12345 after that
	print("FAT0: %u", getFATEntry(0));
	setFATEntry(0, 12345);
}

};