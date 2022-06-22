/*
 * FOSOS keyboard driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <keyboard.h>

#define IRQ_KEYBOARD 1

#define I8042_BUFFER 0x60
#define I8042_STATUS 0x64
#define SET_LEDS 0xED
#define DATA_AVAILABLE 0x01
#define I8042_ACK 0xFA

extern "C" void handleKeyboardInterrupt();
extern "C" void keyboard_isr();

static byte kbd_ledState;

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
	"    call handleKeyboardInterrupt\n"
	"    popw %es\n"
	"    popw %ds\n"
	"    popa\n"
	"    iret\n"
);

#define KBD_MOD_ALT 1
#define KBD_MOD_CTRL 2
#define KBD_MOD_SHIFT 4

PRIVATE char kbdus_map[0x100] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'
};

PRIVATE char kbdus_shift_map[0x100] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'
};

PRIVATE byte kbd_modifiers;

void handleKeyboardInterrupt() {
	while(inb(0x64) & 1) {
		byte ch = inb(0x60);
		switch (ch) {
			case 0x38: kbd_modifiers |= KBD_MOD_ALT; break;
			case 0xB8: kbd_modifiers &= ~KBD_MOD_ALT; break;
			case 0x1D: kbd_modifiers |= KBD_MOD_CTRL; break;
			case 0x9D: kbd_modifiers &= ~KBD_MOD_CTRL; break;
			case 0x2A: kbd_modifiers |= KBD_MOD_SHIFT; break;
			case 0xAA: kbd_modifiers &= ~KBD_MOD_SHIFT; break;
			case 0xFA: /* i8042 ack */ break;
			case 0x1C: /* Enter pressed, notify shell */ shell_handleInput(); break;
			case 0x0E:
				// If at start of line, ignore keypress
				if(shell_inbufPos == 0) break;

				// Move the cursor back and clear the character from display and input buffer
				vga_cursor--;
				shell_inbufPos--;
				vga_setCursor(vga_cursor);
				clearChar(vga_cursor);
				shell_inbuf[shell_inbufPos] = 0;

				break;
			default:
				// Key depressed
				if(ch & 0x80) break;

				if(!kbd_modifiers) ch = kbdus_map[ch];
				else if((kbd_modifiers & KBD_MOD_CTRL) && ch == 46) {
					// Reset the shell
					shell_reset();
					break;
				} else if(kbd_modifiers & KBD_MOD_SHIFT) ch = kbdus_shift_map[ch];

				putChar(ch);
				shell_inbuf[shell_inbufPos++] = ch;
				vga_setCursor(vga_cursor);
		}
	}
	pic_eoi(IRQ_KEYBOARD);
}

void keyboard_init() {
	kbd_modifiers = 0;
	kbd_ledState = 0;

	// Empty the buffer of any pending data
	while (inb(I8042_STATUS ) & DATA_AVAILABLE) inb(I8042_BUFFER);

	// Register our handler and enable it
	registerInterruptHandler(IRQ_VECTOR_BASE + IRQ_KEYBOARD, keyboard_isr);
	pic_enable(IRQ_KEYBOARD);
}

void keyboard_setLED(KeyboardLED led) {
	kbd_ledState |= (byte)led & 7;

	while (inb(I8042_STATUS) & DATA_AVAILABLE);
	outb(I8042_BUFFER, SET_LEDS);
	while (inb(I8042_BUFFER) != I8042_ACK);
	outb(I8042_BUFFER, kbd_ledState);
}

void keyboard_unsetLED(KeyboardLED led) {
	kbd_ledState &= ~((byte)led & 7);

	while (inb(I8042_STATUS) & DATA_AVAILABLE);
	outb(I8042_BUFFER, SET_LEDS);
	while (inb(I8042_BUFFER) != I8042_ACK);
	outb(I8042_BUFFER, kbd_ledState);
}