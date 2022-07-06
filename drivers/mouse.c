/**
 * FOSOS PS2 mouse driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mouse.h>

uint mouseDataState = 0;
byte mouseData[3];
int mouseX = 320;
int mouseY = 240;
#define cursorColor EGA_White
#define cursorWidth 8
#define cursorHeight 8
bool leftBtn = false;

byte cursorImage[] = {
	0b10000000,
	0b11000000,
	0b11100000,
	0b11110000,
	0b11111000,
	0b11100000,
	0b10100000,
	0b00010000
};
byte behindCursor[cursorHeight][cursorWidth];

IRQHANDLER(12);

byte waitAndRead(UInt8 port) {
	for(;;) {
		if(inb(0x64) & 1) break;
	}
	return inb(port);
}

void waitAndWrite(UInt8 port, byte data) {
	for(;;) {
		if(!(inb(0x64) & 2)) break;
	
	}
	outb(port, data);
}

#define read() waitAndRead(0x60);
#define write(data) waitAndWrite(0x64, 0xd4); waitAndWrite(0x60, data);

#define drawCursorImage() { \
	for(uint iy = 0; iy < cursorHeight; iy++) { \
		for(uint ix = 0; ix < cursorWidth; ix++) { \
			byte* addr = (byte*)(videoMemory + mouseX + ix + ((mouseY + iy) * pitch)); \
			behindCursor[iy][ix] = *addr; \
			if((mouseX + ix < screenWidth) && ((cursorImage[iy] << ix) & 0x80)) *addr = cursorColor; \
		} \
	} \
}

#define clearCursorImage(x, y) { \
	for(uint iy = 0; iy < cursorHeight; iy++) { \
		for(uint ix = 0; ix < cursorWidth; ix++) { \
			*(byte*)(videoMemory + x + ix + ((y + iy) * pitch)) = behindCursor[iy][ix]; \
		} \
	} \
}

void drawCursor() {
	drawCursorImage();
}

void handleIRQ12() {
	mouseData[mouseDataState] = inb(0x60);

	if(mouseDataState == 0) {
		mouseDataState++;
	} else if(mouseDataState == 1) {
		mouseDataState++;
	} else if(mouseDataState == 2) {
		mouseDataState = 0;

		int oldX = mouseX;
		int oldY = mouseY;

		mouseX += (Int8)mouseData[2];
		mouseY += (Int8)-mouseData[0];

		if(mouseX < 0) mouseX = 0;
		else if(mouseX >= screenWidth) mouseX = screenWidth - 1;
		if(mouseY < 0) mouseY = 0;
		else if(mouseY >= screenHeight) mouseY = screenHeight - 1;

		if(mouseData[1] & 1) {
			// Left button depressed
			if(!leftBtn) {
				leftBtn = true;
				handleDepress();
			}
		} else if(leftBtn) {
			// Left button released
			leftBtn = false;
			handleRelease();
		}

		clearCursorImage(oldX, oldY);
		drawCursorImage();
	}

	exitIRQHandler(IRQ_PS2MOUSE);
}

void mouse_init() {
	registerIRQHandler(IRQ_PS2MOUSE, isr12);

	// Enable the aux port
	waitAndWrite(0x64, 0xa8);

	// Enable mouse interrupts
	waitAndWrite(0x64, 0x20);

	byte status = waitAndRead(0x60) | 2;
	waitAndWrite(0x64, 0x60);
	waitAndWrite(0x60, status);

	// Use defaults
	write(0xf6);
	byte ack1 = read();
	ASSERT(ack1 == 0xfa);

	// Enable
	write(0xf4);
	byte ack2 = read();
	ASSERT(ack2 == 0xfa);

	enableIRQ(IRQ_PS2MOUSE);

	debugf("PS2 mouse initialized at (%d, %d)\n", mouseX, mouseY);
}