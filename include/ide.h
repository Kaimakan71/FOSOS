/*
 * FOSOS IDE hard disk driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <i386.h>
#include <ports.h>

#define IRQ_IDE0_MASTER 14

#define IDE_DATA   0x1f0
#define IDE_STATUS 0x1f7
#define IDE_CMD    0x1f7

#define IDE_BUSY 0x80
#define IDE_DRDY 0x40
#define IDE_DRQ  0x08

#define IDE_IDENTIFY 0xec
#define IDE_READ 0x21
#define IDE_WRITE 0x30

typedef struct {
	UInt16 type;
	UInt16 cylinders;
	UInt16 heads;
	UInt16 trackSize;
	char serial[36];
	char model[48];
} IDEDrive;

typedef struct {
	UInt32 cylinder;
	UInt16 head;
	UInt16 sector;
} CHS;

extern IDEDrive ide_drives[4];

void ide_read(UInt32 start, UInt16 count, byte* buffer);
void ide_write(UInt32 start, UInt16 count, byte* buffer);
void ide_init();