/*
 * FOSOS IDE driver
 * NOTE: This is BAD polling-based code. I plan to write interrupt based code later.
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <ide.h>

void ide_read(UInt32 lba, byte* buffer) {
	WAIT_BSY();

	outb(0x1f6, 0xe0 | ((lba >> 24) & 0xf));
	outb(0x1f2, 0x01);
	outb(0x1f3, (UInt8)lba);
	outb(0x1f4, (UInt8)(lba >> 8));
	outb(0x1f5, (UInt8)(lba >> 16));
	outb(0x1f7, 0x20);

	UInt16* words = (UInt16*)buffer;

	WAIT_BSY();
	WAIT_RDY();

	for(UInt16 i = 0; i < 256; i++) words[i] = inw(0x1f0);
}

void ide_write(UInt32 lba, byte* buffer) {
	WAIT_BSY();

	outb(0x1f6, 0xe0 | ((lba >> 24) & 0xf));
	outb(0x1f2, 0x01);
	outb(0x1f3, (UInt8)lba);
	outb(0x1f4, (UInt8)(lba >> 8));
	outb(0x1f5, (UInt8)(lba >> 16));
	outb(0x1f7, 0x30);

	UInt32* dwords = (UInt32*)buffer;

	WAIT_BSY();
	WAIT_RDY();

	for(UInt8 i = 0; i < 128; i++) outl(0x1f0, dwords[i]);
}