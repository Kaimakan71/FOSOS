/*
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <vga.h>
#include <i386.h>
#include <keyboard.h>
#include <pic.h>

// Print a banner in a bright color
void banner() {
	vga_setColor(0x0f);
	print("    dBBBBP dBBBBP.dBBBBP   dBBBBP.dBBBBP\n   dBP    dB'.BP BP       dB'.BP BP\n  dBBBP  dB'.BP  `BBBBb  dB'.BP  `BBBBb\n dBP    dB'.BP      dBP dB'.BP      dBP\ndBP    dBBBBP  dBBBBP' dBBBBP  dBBBBP'   v1.0\n\nType 'help' for a list of commands\n");
	vga_setColor(0x07);
}

void init() {
	disableInterrupts();

	vga_setColor(0x07);
	clearScreen();

	pic_init();
	gdt_init();
	idt_init();

	// Register interrupt handlers here :~)
	keyboard_init();

	banner();
	print("> ");

	enableInterrupts();

	// Stay idle until an interrupt comes along
	for(;;) asm("hlt");
}