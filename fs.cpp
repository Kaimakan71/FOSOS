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

#define fat_entries disk_blocks
int fat_bytes;
int fat_blocks;
int fat_entries_per_block;

// We initialize this to -1 readBlocks won't think it already has the FAT
int fat_chunk_nr = -1;
byte* fat_chunk;

// Read blocks from disk
// Returns a pointer to the data (thanks, malloc!)
byte* readRawBlocks(int start, int count) {
	ASSERT(start + count < disk_blocks);
	print("Reading %u blocks from disk starting at %u\n", count, start);

	byte* buffer = (byte*)malloc(BLOCK_SIZE);
	Disk::read((UInt32)(start * block_sectors), (UInt8)(count * block_sectors), buffer);
	return buffer;
}

// Write blocks to disk
void writeRawBlocks(int start, int count, byte* data) {
	ASSERT(start + count < disk_blocks);
	print("Writing %u blocks to disk starting at %u\n", count, start);

	Disk::write((UInt32)(start * block_sectors), (UInt8)(count * block_sectors), data);
}

// Ensure that the loaded FAT block has the entry <entry>
void readFAT(int entry) {
	// ASSERT(entry < fat_entries);

	int block = entry / fat_entries_per_block;
	print("Reading FAT entry %u\n", entry);

	// If we don't have the FAT block we need, get it
	if(fat_chunk_nr != block) {
		print("Loading FAT block %u from disk\n", block);

		// Free the previous chunk if we've read before
		if(fat_chunk_nr != -1) free(fat_chunk);

		// Get the new chunk
		fat_chunk = readRawBlocks(block, 1);
		fat_chunk_nr = block;
	}
}

// Get an entry in the FAT, loading from disk if needed
FATEntry getFATEntry(int entry) {
	readFAT(entry);

	FATEntry* table = (FATEntry*)fat_chunk;
	return table[entry % fat_entries_per_block];
}

// Get an entry in the FAT, write to it and save to disk
void setFATEntry(int entry, FATEntry value) {
	readFAT(entry);

	FATEntry* table = (FATEntry*)fat_chunk;
	table[entry % fat_entries_per_block] = value;

	// Write out to disk
	writeRawBlocks(fat_chunk_nr, 1, fat_chunk);
}

void init() {
	print("Initializing file system :O\n");

	block_sectors = BLOCK_SIZE / SECTOR_SIZE;
	disk_blocks = DISK_BYTES / BLOCK_SIZE;

	fat_bytes = fat_entries * sizeof(FATEntry);
	// Divide and round up: a/b = (a + (b - 1)) / b
	fat_blocks = (fat_bytes + (BLOCK_SIZE - 1)) / BLOCK_SIZE;
	fat_entries_per_block = BLOCK_SIZE / sizeof(FATEntry);

	print("fe=%u, fb=%u, fB=%u, fepB=%u\n",
		fat_entries,
		fat_bytes,
		fat_blocks,
		fat_entries_per_block
	);

	print("FAT0=%u\n", getFATEntry(0));
	setFATEntry(0, 12345);
}

};