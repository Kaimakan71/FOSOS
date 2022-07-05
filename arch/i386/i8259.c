/**
 * i8259 Programmable Interrupt Controller driver for FOSOS
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <i8253.h>

void disableIRQ(UInt8 vector) {
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

void enableIRQ(UInt8 vector) {
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

void pic_init() {
	// Enter edge triggered mode with cascading controllers
	outb(PIC0_CMD, 0x11);
	outb(PIC1_CMD, 0x11);

	// Remap IRQs to avoid exception ISRs
	outb(PIC0_DAT, PIC0_VECTOR_BASE);
	outb(PIC1_DAT, PIC1_VECTOR_BASE);

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
	enableIRQ(SLAVE_INDEX);
}