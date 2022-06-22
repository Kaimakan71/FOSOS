/*
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <pic.h>
#include <i386.h>
#include <keyboard.h>
#include <pit.h>
#include <shell.h>

void init() {
	disableInterrupts();

	// Initialize the Programmable Interrupt Controller
	pic_init();

	// Initialize cpu tables
	gdt_init();
	idt_init();

	// Register interrupt handlers here :~)
	keyboard_init();
	pit_init();

	// Initialize the Real-Time Clock
	rtc_init();

	// Initialize the shell
	shell_init();

	// Enable interrupts
	enableInterrupts();

	// Stay idle until an interrupt comes along
	for(;;) asm("hlt");
}