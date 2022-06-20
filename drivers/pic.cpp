/*
 * Basic PIC controller
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <pic.h>

#define SLAVE_INDEX 2

#define PIC0_CTL 0x20
#define PIC0_CMD 0x21
#define PIC1_CTL 0xA0
#define PIC1_CMD 0xA1

PUBLIC void pic_disable(UInt8 vector) {
	byte imr;
	if(vector & 8) {
		imr = inb(PIC1_CMD);
		imr |= 1 << (vector - 8);
		outb(PIC1_CMD, imr);
	} else {
		imr = inb(PIC0_CMD);
		imr |= 1 << vector;
		outb(PIC0_CMD, imr);
	}
}

PUBLIC void pic_enable(UInt8 vector) {
	byte imr;
	if(vector & 8) {
		imr = inb(PIC1_CMD);
		imr &= ~(1 << (vector - 8));
		outb(PIC1_CMD, imr);
	} else {
		imr = inb(PIC0_CMD);
		imr &= ~(1 << vector);
		outb(PIC0_CMD, imr);
	}
}

PUBLIC void pic_eoi(UInt8 vector) {
	if(vector & 8) outb(PIC1_CTL, 0x20);
	outb(PIC0_CTL, 0x20);
}

PUBLIC void pic_init() {
	// Enter edge triggered mode with cascading controllers
	outb(PIC0_CTL, 0x11);
	outb(PIC1_CTL, 0x11);

	// Remap IRQs to avoid exception ISRs
	outb(PIC0_CMD, IRQ_VECTOR_BASE);
	outb(PIC1_CMD, IRQ_VECTOR_BASE + 0x08);

	// Configure master-slave relationship
	outb(PIC0_CMD, 1 << SLAVE_INDEX);
	outb(PIC1_CMD, SLAVE_INDEX);

	// Enter x86 mode
	outb(PIC0_CMD, 0x01);
	outb(PIC1_CMD, 0x01);

	// Mask all interrupts on both PICs (handlers will unmask them)
	outb(PIC0_CMD, 0xff);
	outb(PIC1_CMD, 0xff);

	// Enable the master-slave communication line
	pic_enable(SLAVE_INDEX);
}