/**
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <assert.h>
#include <video.h>
#include <memory.h>
#include <i8259.h>
#include <cputable.h>
#include <keyboard.h>
#include <ps2mouse.h>
#include <i8253.h>
#include <rtc.h>
#include <gui.h>

void init() {
	// Initialize the memory manager
	memory_init();

	// Initialize the Programmable Interrupt Controller and CPU tables
	pic_init();
	gdt_init();
	idt_init();

	// Register interrupt handlers / initialize drivers
	kbd_init();
	ps2mouse_init();
	pit_init();
	rtc_init();

	// Initialize the display
	gui_init();

	// Enable interrupts
	asm volatile("sti");

	// Stay idle, until we get an interrupt
	for(;;) asm volatile("hlt");
}