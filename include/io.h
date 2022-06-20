/*
 * I/O routines for FOSOS
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>

byte inb(word port);
word inw(word port);
dword ind(word port);

void outb(word port, byte data);
void outw(word port, word data);
void outd(word port, dword data);

byte cmos_read(byte index);
void cmos_write(byte index, byte data);