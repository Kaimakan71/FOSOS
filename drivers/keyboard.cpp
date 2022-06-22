/*
 * FOSOS keyboard driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <keyboard.h>

extern "C" void handle_keyboard();
extern "C" void keyboard_isr();

asm(
	".globl keyboard_isr\n"
	"keyboard_isr:\n"
	"    pusha\n"
	"    pushw %ds\n"
	"    pushw %es\n"
	"    pushw %ss\n"
	"    pushw %ss\n"
	"    popw %ds\n"
	"    popw %es\n"
	"    call handle_keyboard\n"
	"    popw %es\n"
	"    popw %ds\n"
	"    popa\n"
	"    iret\n"
);

namespace Keyboard {

static byte ledState;
byte modifiers;

char map_us[0x100] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08, 0,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

char map_us_shift[0x100] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '
};

void setLED(LED led) {
	ledState |= (byte)led & 7;

	while(inb(I8042_STATUS) & DATA_AVAILABLE);
	outb(I8042_BUFFER, SET_LEDS);
	while(inb(I8042_BUFFER) != I8042_ACK);
	outb(I8042_BUFFER, ledState);
}

void unsetLED(LED led) {
	ledState &= ~((byte)led & 7);

	while(inb(I8042_STATUS) & DATA_AVAILABLE);
	outb(I8042_BUFFER, SET_LEDS);
	while(inb(I8042_BUFFER) != I8042_ACK);
	outb(I8042_BUFFER, ledState);
}

void init() {
	modifiers = 0;
	ledState = 0;

	// Empty the buffer of any pending data
	while(inb(I8042_STATUS ) & DATA_AVAILABLE) inb(I8042_BUFFER);

	// Register our handler and enable it
	registerInterruptHandler(IRQ_VECTOR_BASE + IRQ_KEYBOARD, keyboard_isr);
	PIC::enable(IRQ_KEYBOARD);
}

};

void handle_keyboard() {
	// Read and interpert until the buffer is empty again
	while(inb(0x64) & 1) {
		byte character = inb(0x60);
		switch (character) {
			case 0x38: Keyboard::modifiers |= KBD_MOD_ALT; break;
			case 0xB8: Keyboard::modifiers &= ~KBD_MOD_ALT; break;
			case 0x1D: Keyboard::modifiers |= KBD_MOD_CTRL; break;
			case 0x9D: Keyboard::modifiers &= ~KBD_MOD_CTRL; break;
			case 0x2A: Keyboard::modifiers |= KBD_MOD_SHIFT; break;
			case 0xAA: Keyboard::modifiers &= ~KBD_MOD_SHIFT; break;
			case 0xFA: /* i8042 ack */ break;
			case 0x1C: /* Enter pressed, notify shell */ Shell::handleInput(); break;
			case 0x0E:
				// If at start of line, ignore keypress
				if(Shell::inbufPos == 0) break;

				// Move the cursor back and clear the character from display and input buffer
				VGA::cursor--;
				Shell::inbufPos--;
				VGA::setCursor(VGA::cursor);
				clearChar(VGA::cursor);
				Shell::inbuf[Shell::inbufPos] = 0;

				break;
			default:
				// Key depressed
				if(character & 0x80) break;

				if(!Keyboard::modifiers) character = Keyboard::map_us[character];
				else if((Keyboard::modifiers & KBD_MOD_CTRL) && character == 46) {
					// Reset the shell
					Shell::reset();
					break;
				} else if(Keyboard::modifiers & KBD_MOD_SHIFT) character = Keyboard::map_us_shift[character];

				putChar(character);
				Shell::inbuf[Shell::inbufPos++] = character;
				VGA::setCursor(VGA::cursor);
		}
	}
	PIC::eoi(IRQ_KEYBOARD);
}