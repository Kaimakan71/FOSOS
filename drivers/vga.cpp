/*
 * VGA text mode driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <vga.h>

PRIVATE byte *vga_mem = (byte*)0xb8000;
PRIVATE byte vga_color = 0x07;
PUBLIC volatile UInt16 vga_cursor;

PUBLIC byte vga_getColor() {
	return vga_color;
}

PUBLIC void vga_setColor(byte color) {
	vga_color = color;
}

PUBLIC UInt16 vga_getCursor() {
	word value;
	outb(0x3d4, 0x0e);
	value = inb(0x3d5) << 8;
	outb(0x3d4, 0x0f);
	value |= inb(0x3d5);
	return value;
}

PUBLIC void vga_setCursor(UInt16 value) {
	if(value >= VGA_CHARS) {
		vga_setCursor(0);
		vga_color = 0x0c;
		return;
	}
	outb(0x3d4, 0x0e);
	outb(0x3d5, MSB(value));
	outb(0x3d4, 0x0f);
	outb(0x3d5, LSB(value));
}

PUBLIC void vga_setCursor(UInt8 x, UInt8 y) {
	vga_setCursor(y * 80 + x);
}

PUBLIC void putChar(char character) {
	UInt16 row;

	switch(character) {
		case '\n':
			row = vga_cursor / VGA_WIDTH;
			if(row == 23) {
				memcpy(vga_mem, vga_mem + 160, 160 * 23);
				memset(vga_mem + (160 * 23), 0, 160);
				vga_cursor = row * VGA_WIDTH;
			} else
				vga_cursor = (row + 1) * VGA_WIDTH;
			return;
		default:
			vga_mem[vga_cursor * 2] = character;
			vga_mem[vga_cursor * 2 + 1] = vga_color;
			vga_cursor++;
	}
	row = vga_cursor / VGA_WIDTH;
	if(row >= 24) {
		memcpy(vga_mem, vga_mem + 160, 160 * 23);
		memset(vga_mem + (160 * 23), 0, 160);
		vga_cursor = 23 * VGA_WIDTH;
	}
}

PUBLIC void putCharAt(char character, UInt8 x, UInt8 y) {
	UInt16 pos = (y * 160) + (x * 2);
	vga_mem[pos] = character;
	vga_mem[pos + 1] = vga_color;
}

PUBLIC void fillRect(UInt8 x, UInt8 y, UInt8 width, UInt8 height, byte color) {
	for(UInt8 iy = 0; iy < height; iy++) {
		UInt16 y_pos = (y + iy) * 160;
		for(UInt8 ix = 0; ix < width; ix++) {
			UInt16 pos = y_pos + (x + ix) * 2;
			vga_mem[pos] = ' ';
			vga_mem[pos + 1] = color;
		}
	}
}

PUBLIC void clearChar(UInt16 index) {
	vga_mem[index * 2] = ' ';
}

PRIVATE void printHex(UInt32 number, UInt8 fields) {
	byte shr_count = fields * 4;
	while(shr_count) {
		shr_count -= 4;
		putChar(hexChars[(number >> shr_count) & 0x0F]);
	}
}

PRIVATE void printNum(UInt32 number) {
	dword divisor = 1000000000;
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

PRIVATE void printSignedNumber(int number) {
	if(number < 0) {
		putChar('-');
		printNum(0 - number);
	} else printNum(number);
}

PUBLIC void print(const char *fmt, ...) {
	const char *p;
	va_list ap;

	vga_cursor = vga_getCursor();

	va_start( ap, fmt );

	for(p = fmt; *p; ++p) {
		if(*p == '%' && *(p + 1)) {
			++p;
			switch(*p) {
				case 's':
					{
						const char* sp = va_arg(ap, const char*);
						//ASSERT(sp != nullptr);
						if(!sp) {
							putChar('<');
							putChar('N');
							putChar('u');
							putChar('L');
							putChar('>');
						} else for(; *sp; ++sp) putChar(*sp);
					}
					break;
				case 'd':
					printSignedNumber(va_arg(ap, int));
					break;

				case 'u':
					printNum(va_arg( ap, dword ));
					break;

				case 'x':
					printHex(va_arg( ap, dword ), 8);
					break;

				case 'b':
					printHex(va_arg( ap, int ), 2);
					break;

				case 'c':
					putChar((char)va_arg( ap, int ));
					break;

				case 'p':
					putChar('0');
					putChar('x');
					printHex(va_arg( ap, dword ), 8);
					break;
			}
		}
		else putChar(*p);
	}

	va_end(ap);

	// Update the cursor
	vga_setCursor(vga_cursor);
}

PUBLIC void clearScreen() {
	for(UInt16 i = 0; i < VGA_CHARS; ++i) {
		vga_mem[i*2] = ' ';
		vga_mem[i*2 + 1] = vga_color;
	}

	vga_setCursor(0);
}