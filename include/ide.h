/*
 * FOSOS IDE driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <io.h>

// We should really detect this, but I'll assume for now
#define DISK_BYTES (64 * 1024)

#define IDE_BSY 0x80
#define IDE_RDY 0x40
#define IDE_DF  0x20
#define IDE_DRQ 0x08
#define IDE_ERR 0x01

#define WAIT_BSY() while(inb(0x1f7) & IDE_BSY);
#define WAIT_RDY() while(!(inb(0x1f7) & IDE_RDY));

namespace Disk {

void read(UInt32 start, UInt8 count, byte* buffer);
void write(UInt32 start, UInt8 count, byte* buffer);

};