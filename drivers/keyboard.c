/**
 * FOSOS keyboard driver
 * Currently US-only (plans for more later)
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <keyboard.h>

IRQHANDLER(1);

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

byte kbd_modifiers;

void handleIRQ1() {
	// Read and interpert until the buffer is empty again
	while(inb(0x64) & 1) {
		byte character = inb(0x60);
		switch (character) {
			case 0x38: kbd_modifiers |= KBD_MOD_ALT; break;
			case 0xB8: kbd_modifiers &= ~KBD_MOD_ALT; break;
			case 0x1D: kbd_modifiers |= KBD_MOD_CTRL; break;
			case 0x9D: kbd_modifiers &= ~KBD_MOD_CTRL; break;
			case 0x2A: kbd_modifiers |= KBD_MOD_SHIFT; break;
			case 0xAA: kbd_modifiers &= ~KBD_MOD_SHIFT; break;
			case 0xFA: /* i8042 ack */ break;
			default:
				// Key depressed
				if(character & 0x80) break;

				if(!kbd_modifiers) character = map_us[character];
				else if(kbd_modifiers & KBD_MOD_SHIFT) character = map_us_shift[character];
		}
	}

	exitIRQHandler(IRQ_KEYBOARD);
}

void kbd_init() {
	kbd_modifiers = 0;

	// Empty the buffer of any pending data
	while(inb(I8042_STATUS ) & DATA_AVAILABLE) inb(I8042_BUFFER);

	// Register our handler and enable it
	registerIRQHandler(IRQ_KEYBOARD, isr1);
	enableIRQ(IRQ_KEYBOARD);
}