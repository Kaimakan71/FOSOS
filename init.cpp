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
#include <shell.h>

void init() {
	disableInterrupts();

	vga_setColor(0x07);
	clearScreen();

	pic_init();
	gdt_init();
	idt_init();

	// Register interrupt handlers here :~)
	keyboard_init();

	shell_init();

	enableInterrupts();

	// Stay idle until an interrupt comes along
	for(;;) asm("hlt");
}