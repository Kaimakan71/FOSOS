/**
 * FOSOS VBE/VGA video driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <video.h>

VBEModeInfo* mode;
byte* videoMemory;

uint screenWidth;
uint screenHeight;
uint bitDepth;
UInt16 pitch;

const char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
uint cursorPos = 0;
uint lineSpacing = 2;
byte textColor = EGA_LGray;
uint charsPerLine;

// Traditional fill rect
// x, y, width, height, color
void fillRect(uint x, uint y, uint width, uint height, Color color) {
	for(uint iy = 0; iy < height; iy++) {
		for(uint ix = 0; ix < width; ix++) {
			*(videoMemory + x + ix + ((y + iy) * pitch)) = color;
		}
	}
}

// Faster than fillRect
// Only uses one loop, instead of two
// Also has less calculations to make
void fillScreen(Color color) {
	for(uint i = 0; i < (pitch * screenHeight); i++) {
		*(videoMemory + i) = color;
	}
}

// Draw a perfectly flat horizontal line
void drawHLine(uint x, uint y, uint width, Color color) {
	for(uint i = 0; i < width; i++) {
		*(videoMemory + x + i + (y * pitch)) = color;
	}
}

// Draw a perfectly flat vertical line
void drawVLine(uint x, uint y, uint height, Color color) {
	for(uint i = 0; i < height; i++) {
		*(videoMemory + x + ((y + i) * pitch)) = color;
	}
}

// Draw a character from a bitmap font
void drawChar(char character, uint x, uint y, Color color) {
	for(uint iy = 0; iy < charHeight; iy++) {
		for(uint ix = 0; ix < charWidth; ix++) {
			if((character - 33 >= 0) && ((font[character - 33][iy] >> ix) & 1)) {
				*(videoMemory + x + (charWidth - ix - 1) + ((y + iy) * pitch)) = color;
			}
		}
	}
}

#define putChar(character) outb(0x3f8, character);

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

void debugf(const char* fmt, ... ) {
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
}

void error(const char* msg) {
	textColor = EGA_LRed;
	debugf("Error: %s", msg);
	textColor = EGA_LGray;
}

void video_init() {
	mode = (VBEModeInfo*)0x2000;
	videoMemory = (byte*)(mode->framebuffer);

	// Useful numbers
	screenWidth  = mode->width;
	screenHeight = mode->height;
	bitDepth     = mode->bpp;
	charsPerLine = screenWidth / charWidth;
	pitch        = mode->pitch;

	debugf("Using %ux%ux%ubpp VBE framebuffer @%x\n",
		screenWidth,
		screenHeight,
		bitDepth,
		videoMemory
	);
}