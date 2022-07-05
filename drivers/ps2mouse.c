/**
 * FOSOS PS2 mouse driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <ps2mouse.h>

uint mouseDataState = 0;
byte mouseData[3];
int mouseX = 320;
int mouseY = 240;

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

		*(byte*)(videoMemory + oldX + (oldY * 640)) = VGA_Gray + 1;
		*(byte*)(videoMemory + mouseX + (mouseY * 640)) = EGA_White;
	}

	exitIRQHandler(IRQ_PS2MOUSE);
}

void ps2mouse_init() {
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
}