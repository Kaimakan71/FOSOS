/**
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <assert.h>
#include <vga.h>
#include <rtc.h>
#include <pic.h>
#include <i386.h>
#include <keyboard.h>
#include <pit.h>
#include <shell.h>

void init() {
	// Initialize things we need for debugging
	vga_init();
	printf("Starting FOSOS...\n");

	// Initialize the real-time clock
	rtc_init();

	// Initialize the Programmable Interrupt Controller and CPU tables
	pic_init();
	gdt_init();
	idt_init();

	// Register interrupt handlers / initialize drivers
	kbd_init();
	pit_init();
	pic_enable(IRQ_TIMER);

	// Initialize the shell
	shell_init();

	// Enable interrupts, activating drivers
	enableInterrupts();

	// Stay idle, until we get an interrupt
	for(;;) asm volatile("hlt");
}