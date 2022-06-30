/**
 * i8259 Programmable Interrupt Controller driver for FOSOS
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <pic.h>

void pic_disable(UInt8 vector) {
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

void pic_enable(UInt8 vector) {
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

void pic_eoi(UInt8 vector) {
	if(vector & 8) outb(PIC1_CMD, 0x20);
	outb(PIC0_CMD, 0x20);
}

// Dump the IRR and ISR registers
// I think this works?
void pic_dumpRegs() {
	outb(PIC0_CMD, 0x0a);
	outb(PIC1_CMD, 0x0a);
	byte irr = (inb(PIC1_CMD) << 8) | inb(PIC0_CMD);
	outb(PIC0_CMD, 0x0b);
	outb(PIC1_CMD, 0x0b);
	byte isr = (inb(PIC1_CMD) << 8) | inb(PIC0_CMD);
	printf("IRR=%x ISR=%x", irr, isr);
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
	pic_enable(SLAVE_INDEX);
	printf("[PIC] i8259 initialized: cascading mode, vector bases %b/%b\n", PIC0_VECTOR_BASE, PIC1_VECTOR_BASE);
}