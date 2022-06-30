/**
 * FOSOS keyboard driver
 * Currently US-only (plans for more later)
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <keyboard.h>

extern void handle_keyboard();
extern void keyboard_isr();
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

void handle_keyboard() {
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
			case 0x1c:
				/* Enter pressed, notify shell */
				shell_handleInput();
				break;
			case 0x0e:
				/* Backspace pressed */
				// If at start of line, ignore keypress
				if(shell_inbufPos == 0) break;

				// Move the cursor back and clear the character from display and input buffer
				vga_cursor--;
				shell_inbufPos--;
				vga_updateCursor();
				clearChar(vga_cursor);
				shell_inbuf[shell_inbufPos] = 0;
				break;
			default:
				// Key depressed
				if(character & 0x80) break;

				if(!kbd_modifiers) character = map_us[character];
				else if((kbd_modifiers & KBD_MOD_CTRL) && character == 46) {
					// Reset the shell
					shell_reset();
					break;
				} else if(kbd_modifiers & KBD_MOD_SHIFT) character = map_us_shift[character];

				putChar(character);
				shell_inbuf[shell_inbufPos++] = character;
				vga_updateCursor();
		}
	}
	pic_eoi(IRQ_KEYBOARD);
}

void kbd_init() {
	kbd_modifiers = 0;

	// Empty the buffer of any pending data
	while(inb(I8042_STATUS ) & DATA_AVAILABLE) inb(I8042_BUFFER);

	// Register our handler and enable it
	registerInterruptHandler(IRQ_VECTOR_BASE + IRQ_KEYBOARD, keyboard_isr);
	pic_enable(IRQ_KEYBOARD);
}