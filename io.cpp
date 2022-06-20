/*
 * I/O routines for FOSOS
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <io.h>

byte inb(word port) {
	byte value;
	asm("inb %%dx, %%al":"=a"(value):"d"(port));
	return value;
}

word inw(word port) {
	word value;
	asm("inw %%dx, %%ax":"=a"(value):"d"(port));
	return value;
}

dword ind(dword port) {
	dword value;
	asm("inl %%dx, %%eax":"=a"(value):"d"(port));
	return value;
}

void outb(word port, byte value) {
	asm("outb %%al, %%dx"::"d"(port), "a"(value));
}

void outw(word port, word value) {
	asm("outw %%ax, %%dx"::"d"(port), "a"(value));
}

void outd(word port, word value) {
	asm("outl %%eax, %%dx"::"d"(port), "a"(value));
}

byte cmos_read(byte index) {
	outb(0x70, index);
	return inb(0x71);
}

void cmos_write(byte index, byte data) {
	outb(0x70, index);
	outb(0x71, data);
}