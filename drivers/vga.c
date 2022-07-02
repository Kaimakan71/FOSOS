/**
 * FOSOS VGA text mode driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <vga.h>

const char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
const char hexCharsUpper[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
byte* vga_mem = (byte*)0xb8000;
byte vga_color = 0x07;
volatile UInt16 vga_cursor;

byte vga_getColor() {
	return vga_color;
}

void vga_setColor(UInt8 fg, UInt8 bg) {
	vga_color = (bg << 4) | (fg & 0x0f);
}

UInt16 vga_getCursor() {
	UInt16 value;
	outb(0x3d4, 0x0e);
	value = inb(0x3d5) << 8;
	outb(0x3d4, 0x0f);
	value |= inb(0x3d5);
	return value;
}

void vga_updateCursor() {
	outb(0x3d4, 0x0e);
	outb(0x3d5, MSB(vga_cursor));
	outb(0x3d4, 0x0f);
	outb(0x3d5, LSB(vga_cursor));
}

void putChar(char character) {
	if(character == '\n') vga_cursor += VGA_WIDTH - (vga_cursor % VGA_WIDTH);
	else if(character == '\r') vga_cursor -= vga_cursor % VGA_WIDTH;
	else {
		vga_mem[vga_cursor * 2] = character;
		vga_mem[vga_cursor * 2 + 1] = vga_color;
		vga_cursor++;
	}

	if((vga_cursor / VGA_WIDTH) == VGA_HEIGHT) {
		memcpy(vga_mem, vga_mem + 160, 160 * 24);

		word* linemem = (word*)&vga_mem[24 * 160];
		for(UInt8 i = 0; i < 80; i++) linemem[i] = 0x0720;
		vga_cursor = 24 * VGA_WIDTH;
		vga_updateCursor();
	}
}

void clearChar(UInt16 index) {
	vga_mem[index * 2] = ' ';
}

void putCharAt(char character, UInt8 x, UInt8 y) {
	UInt16 pos = (y * 160) + (x * 2);
	vga_mem[pos] = character;
	vga_mem[pos + 1] = vga_color;
}

void printUInt(UInt32 number) {
	UInt32 divisor = 1000000000;
	char character;
	char padding = 1;

	for(;;) {
		character = '0' + (number / divisor);
		number %= divisor;

		if(character != '0') padding = 0;
		if(!padding || divisor == 1) putChar(character);

		if(divisor == 1) break;
		divisor /= 10;
	}
}

void printInt(int number) {
	if(number < 0) {
		putChar('-');
		printUInt(0 - number);
	} else printUInt(number);
}

void printHex(UInt32 number, UInt8 fields) {
	putChar('0');
	putChar('x');
	byte shr_count = fields * 4;
	while(shr_count) {
		shr_count -= 4;
		putChar(hexChars[(number >> shr_count) & 0x0f]);
	}
}

void printHexUpper(UInt32 number, UInt8 fields) {
	putChar('0');
	putChar('X');
	byte shr_count = fields * 4;
	while(shr_count) {
		shr_count -= 4;
		putChar(hexCharsUpper[(number >> shr_count) & 0x0f]);
	}
}

void printf(const char* fmt, ... ) {
	const char* p;
	va_list ap;
	va_start(ap, fmt);

	for(p = fmt; *p; p++) {
		if(*p == '%' && *(p + 1)) {
			p++;
			switch(*p) {
				case 's':
					// String
					{
						const char* sp = va_arg(ap, const char*);
						if(!sp) {
							putChar('<');
							putChar('N');
							putChar('u');
							putChar('L');
							putChar('>');
						} else while(*sp != '\0') putChar(*sp++);
					}
					break;
				case 'd':
				case 'i':
					// Signed integer
					printInt(va_arg(ap, int));
					break;
				case 'u':
					// Unsigned integer
					printUInt(va_arg(ap, UInt32));
					break;
				case 'x':
					// Hexadecimal, lowercase
					printHex(va_arg(ap, UInt32), 8);
					break;
				case 'X':
					// Hexadecimal, lowercase
					printHexUpper(va_arg(ap, UInt32), 8);
					break;
				case 'b':
					// Hex byte
					printHex(va_arg(ap, UInt32), 2);
					break;
				case 'c':
					// Character
					putChar((char)va_arg(ap, int));
					break;
				case 'p':
					// Number - padded to 2 characters
					{
						UInt32 value = va_arg(ap, UInt32);
						if(value < 10) putChar('0');
						printUInt(value);
					}
					break;
			}
		} else putChar(*p);
	}

	va_end(ap);
	vga_updateCursor();
}

void clearScreen() {
	for(UInt16 i = 0; i < VGA_CHARS; i++) {
		vga_mem[i * 2] = ' ';
		vga_mem[i * 2 + 1] = 0x07;
	}
	vga_cursor = 0;
	vga_updateCursor();
}

void printCenter(const char* string) {
	vga_cursor += (VGA_WIDTH / 2) - (strlen(string) / 2);
	printf(string);
}

void panic(const char* message) {
	clearScreen();
	vga_cursor = VGA_WIDTH * (VGA_HEIGHT / 2 - 1);

	vga_setColor(0xf, 0x0);
	printCenter("KERNEL PANIC:\n");

	vga_setColor(0xf, 0x4);
	printCenter(message);

	// Get ready for debug dump
	vga_cursor = 0;
	vga_setColor(0xf, 0x0);
	vga_updateCursor();
}

void error(const char* message) {
	vga_setColor(0xc, 0x0);
	printf("Error: %s", message);
	vga_setColor(0x7, 0x0);
}

void debugf(const char* msg) {
	while(*msg) outb(0x3f8, *msg++);
}

void vga_init() {
	vga_cursor = vga_getCursor();
}