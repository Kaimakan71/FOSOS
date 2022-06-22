/*
 * VGA text mode driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <vga.h>

namespace VGA {

byte *mem = (byte*)0xb8000;
byte color = 0x07;
volatile UInt16 cursor;

byte getColor() {
	return color;
}

void setColor(byte newColor) {
	color = newColor;
}

UInt16 getCursor() {
	word value;
	outb(0x3d4, 0x0e);
	value = inb(0x3d5) << 8;
	outb(0x3d4, 0x0f);
	value |= inb(0x3d5);
	return value;
}

void setCursor(UInt16 value) {
	if(value >= VGA_CHARS) {
		setCursor(0);
		color = 0x0c;
		return;
	}
	outb(0x3d4, 0x0e);
	outb(0x3d5, MSB(value));
	outb(0x3d4, 0x0f);
	outb(0x3d5, LSB(value));
}

void setCursor(UInt8 x, UInt8 y) {
	setCursor(y * 80 + x);
}

void printHex(UInt32 number, UInt8 fields) {
	byte shr_count = fields * 4;
	while(shr_count) {
		shr_count -= 4;
		putChar(hexChars[(number >> shr_count) & 0x0F]);
	}
}

void printNum(UInt32 number) {
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

void printSignedNumber(int number) {
	if(number < 0) {
		putChar('-');
		printNum(0 - number);
	} else printNum(number);
}

void clearScreen() {
	for(UInt16 i = 0; i < VGA_CHARS; ++i) {
		mem[i*2] = ' ';
		mem[i*2 + 1] = color;
	}

	setCursor(0);
}

};

void putChar(char character) {
	UInt16 row;

	switch(character) {
		case '\n':
			row = VGA::cursor / VGA_WIDTH;
			if(row == 23) {
				memcpy(VGA::mem, VGA::mem + 160, 160 * 23);
				memset(VGA::mem + (160 * 23), 0, 160);
				VGA::cursor = row * VGA_WIDTH;
			} else VGA::cursor = (row + 1) * VGA_WIDTH;
			return;
		default:
			VGA::mem[VGA::cursor * 2] = character;
			VGA::mem[VGA::cursor * 2 + 1] = VGA::color;
			VGA::cursor++;
	}
	row = VGA::cursor / VGA_WIDTH;
	if(row >= 24) {
		memcpy(VGA::mem, VGA::mem + 160, 160 * 23);
		memset(VGA::mem + (160 * 23), 0, 160);
		VGA::cursor = 23 * VGA_WIDTH;
	}
}

void putCharAt(char character, UInt8 x, UInt8 y) {
	UInt16 pos = (y * 160) + (x * 2);
	VGA::mem[pos] = character;
	VGA::mem[pos + 1] = VGA::color;
}

void fillRect(UInt8 x, UInt8 y, UInt8 width, UInt8 height, byte color) {
	for(UInt8 iy = 0; iy < height; iy++) {
		UInt16 y_pos = (y + iy) * 160;
		for(UInt8 ix = 0; ix < width; ix++) {
			UInt16 pos = y_pos + (x + ix) * 2;
			VGA::mem[pos] = ' ';
			VGA::mem[pos + 1] = color;
		}
	}
}

void clearChar(UInt16 index) {
	VGA::mem[index * 2] = ' ';
}

void print(const char *fmt, ...) {
	const char *p;
	va_list ap;

	VGA::cursor = VGA::getCursor();

	va_start( ap, fmt );

	for(p = fmt; *p; ++p) {
		if(*p == '%' && *(p + 1)) {
			++p;
			switch(*p) {
				case 's':
					// String
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
					VGA::printSignedNumber(va_arg(ap, int));
					break;

				case 'u':
					VGA::printNum(va_arg(ap, dword));
					break;

				case 'x':
					VGA::printHex(va_arg(ap, dword), 8);
					break;

				case 'b':
					VGA::printHex(va_arg(ap, int), 2);
					break;

				case 'c':
					putChar((char)va_arg(ap, int));
					break;

				case 'p':
					putChar('0');
					putChar('x');
					VGA::printHex(va_arg(ap, dword), 8);
					break;
			}
		}
		else putChar(*p);
	}

	va_end(ap);

	// Update the cursor
	VGA::setCursor(VGA::cursor);
}