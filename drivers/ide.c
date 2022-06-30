/*
 * FOSOS IDE hard disk driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <ide.h>

IDEDrive ide_drives[4];

void ide_init() {
	// When controller ready, send command
	while(inb(IDE_STATUS) & IDE_BUSY);
	outb(0x1f6, 0xa0);
	outb(IDE_CMD, IDE_IDENTIFY);

	// Wait until controller ready with data
	while(inb(IDE_STATUS) & IDE_BUSY);
	while(!(inb(IDE_STATUS) & IDE_DRDY));

	// Buffers and pointers
	byte bbuffer[512];
	byte wbuffer[512];
	byte* b = (byte*)bbuffer;
	word* w = (word*)wbuffer;
	word* words = (word*)wbuffer;

	// Fill buffer
	for(UInt16 i = 0; i < 256; i++) {
		word data = inw(IDE_DATA);
		*(w++) = data;
		*(b++) = MSB(data);
		*(b++) = LSB(data);
	}

	// Unpad model string (spaces to \0)
	for(UInt32 i = 93; i > 54 && bbuffer[i] == ' '; i--) bbuffer[i] = 0;

	// Transfer data to drive table
	ide_drives[0].type = words[0];
	ide_drives[0].cylinders = words[1];
	ide_drives[0].heads = words[3];
	ide_drives[0].trackSize = words[6];
	strcpy(ide_drives[0].serial, (char*)(bbuffer + 20));
	strcpy(ide_drives[0].model, (char*)(bbuffer + 54));

	// A little debug print :~)
	printf("[IDE] Bus 0 master: '%s'\n", ide_drives[0].model);
	printf("[IDE] C=%u/H=%u/Spt=%u, serial no. %s\n",
		ide_drives[0].cylinders,
		ide_drives[0].heads,
		ide_drives[0].trackSize,
		ide_drives[0].serial
	);
}

static CHS LBAToCHS(UInt8 driveNo, UInt32 lba) {
	IDEDrive drive = ide_drives[driveNo];
	CHS chs;
	chs.cylinder = lba / (drive.trackSize * drive.heads);
	chs.head = (lba / drive.trackSize) % drive.heads;
	chs.sector = (lba % drive.trackSize) + 1;
	return chs;
}

void ide_read(UInt32 start, UInt16 count, byte* buffer) {
	CHS chs = LBAToCHS(0, start);

	// Wait until controller ready for options
	while(inb(IDE_STATUS) & IDE_BUSY);

	// Set options
	outb(0x1f2, count == 256 ? 0:LSB(count));
	outb(0x1f3, chs.sector);
	outb(0x1f4, LSB(chs.cylinder));
	outb(0x1f5, MSB(chs.cylinder));
	outb(0x1f6, 0xa0 | chs.head);

	// When controller ready, send request
	outb(0x3f6, 0x08);
	while(!(inb(IDE_STATUS) & IDE_DRDY));
	outb(IDE_CMD, IDE_READ);

	// Wait until controller ready with data
	while(inb(IDE_STATUS) & IDE_BUSY);
	while(!(inb(IDE_STATUS) & IDE_DRDY));

	// Recieve data
	for(UInt32 i = 0; i < (count * 512); i += 2) {
		word w = inw(IDE_DATA);
		buffer[i] = LSB(w);
		buffer[i+1] = MSB(w);
	}
}

void ide_write(UInt32 start, UInt16 count, byte* buffer) {
	CHS chs = LBAToCHS(0, start);

	// Wait until controller ready for options
	while(inb(IDE_STATUS) & IDE_BUSY);

	// Set options
	outb(0x1f2, count == 256 ? 0:LSB(count));
	outb(0x1f3, chs.sector);
	outb(0x1f4, LSB(chs.cylinder));
	outb(0x1f5, MSB(chs.cylinder));
	outb(0x1f6, 0xa0 | chs.head);

	// When controller ready, send request
	outb(0x3f6, 0x08);
	while(!(inb(IDE_STATUS) & IDE_DRDY));
	outb(IDE_CMD, IDE_WRITE);

	// Wait until controller ready for data
	while(inb(IDE_STATUS) & IDE_BUSY);
	while(!(inb(IDE_STATUS) & IDE_DRDY));

	// Send data
	for(UInt32 i = 0; i < (count * 512); i += 4) outl(IDE_DATA, buffer[i] | (buffer[i+1] << 8) | (buffer[i+2] << 16) | (buffer[i+3] << 24));
}