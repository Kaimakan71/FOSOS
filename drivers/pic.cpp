/*
 * Basic PIC controller
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <pic.h>

namespace PIC {

void disable(UInt8 vector) {
	byte imr;
	if(vector & 8) {
		imr = inb(PIC1_DAT);
		imr |= 1 << (vector - 8);
		outb(PIC1_DAT, imr);
	} else {
		imr = inb(PIC0_DAT);
		imr |= 1 << vector;
		outb(PIC0_DAT, imr);
	}
}

void enable(UInt8 vector) {
	byte imr;
	if(vector & 8) {
		imr = inb(PIC1_DAT);
		imr &= ~(1 << (vector - 8));
		outb(PIC1_DAT, imr);
	} else {
		imr = inb(PIC0_DAT);
		imr &= ~(1 << vector);
		outb(PIC0_DAT, imr);
	}
}

void eoi(UInt8 vector) {
	if(vector & 8) outb(PIC1_CMD, 0x20);
	outb(PIC0_CMD, 0x20);
}

void init() {
	// Enter edge triggered mode with cascading controllers
	outb(PIC0_CMD, 0x11);
	outb(PIC1_CMD, 0x11);

	// Remap IRQs to avoid exception ISRs
	outb(PIC0_DAT, IRQ_VECTOR_BASE);
	outb(PIC1_DAT, IRQ_VECTOR_BASE + 0x08);

	// Configure master-slave relationship
	outb(PIC0_DAT, 1 << SLAVE_INDEX);
	outb(PIC1_DAT, SLAVE_INDEX);

	// Enter x86 mode
	outb(PIC0_DAT, 0x01);
	outb(PIC1_DAT, 0x01);

	// Mask all interrupts on both PICs (handlers will unmask them)
	outb(PIC0_DAT, 0xff);
	outb(PIC1_DAT, 0xff);

	// Enable the master-slave communication line
	enable(SLAVE_INDEX);
}

};