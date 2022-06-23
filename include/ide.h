/*
 * FOSOS IDE driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <io.h>

#define IDE_BSY 0x80
#define IDE_RDY 0x40
#define IDE_DF  0x20
#define IDE_DRQ 0x08
#define IDE_ERR 0x01

#define WAIT_BSY() while(inb(0x1f7) & IDE_BSY);
#define WAIT_RDY() while(!(inb(0x1f7) & IDE_RDY));

void ide_read(UInt32 lba, byte* buffer);
void ide_write(UInt32 lba, byte* buffer);