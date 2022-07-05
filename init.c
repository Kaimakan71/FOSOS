/**
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <assert.h>
#include <memory.h>
#include <i8259.h>
#include <cputable.h>
#include <keyboard.h>
#include <i8253.h>
#include <rtc.h>
#include <gui.h>

void init() {
	// Clear the host console
	debugf("\033c");
	debugf("Starting FOSOS...\n");

	// Initialize the memory manager
	memory_init();

	// Initialize the Programmable Interrupt Controller and CPU tables
	pic_init();
	gdt_init();
	idt_init();

	// Initialize essential drivers
	kbd_init();
	pit_init();
	rtc_init();

	// Initialize the GUI
	gui_init();

	createWindow("Launcher", 160, 120, 320, 200, theme.primary);

	// Enable interrupts
	asm volatile("sti");

	// Stay idle, until we get an interrupt
	for(;;) asm volatile("hlt");
}