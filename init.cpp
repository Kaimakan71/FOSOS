/*
 * FOSOS kernel initialization
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <types.h>
#include <i386.h>
#include <rtc.h>
#include <pic.h>
#include <keyboard.h>
#include <pit.h>
#include <shell.h>

void init() {
	disableInterrupts();

	// Initialize the Real-Time Clock
	RTC::init();

	// Initialize the Programmable Interrupt Controller and CPU tables
	PIC::init();
	GDT::init();
	IDT::init();

	// Register interrupt handlers here :~)
	Keyboard::init();
	PIT::init();

	// Initialize the shell
	Shell::init();

	// Enable interrupts
	enableInterrupts();

	// Stay idle until an interrupt comes along
	for(;;) asm("hlt");
}