/**
 * FOSOS port I/O functions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <stdarg.h>

byte inb(UInt16 port);
word inw(UInt16 port);
dword inl(UInt16 port);
void outb(UInt16 port, byte value);
void outw(UInt16 port, word value);
void outl(UInt16 port, dword value);
byte cmos_read(UInt8 index);
void cmos_write(UInt8 index, byte data);