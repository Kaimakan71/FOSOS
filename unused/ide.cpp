/*
 * FOSOS IDE driver
 * A lot of this is from https://wiki.osdev.org/PCI_IDE_Controller
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <ide.h>

extern "C" void disk_isr();
extern "C" void disk_handler();

asm(
	".globl disk_isr\n"
	"disk_isr:\n"
	"    pusha\n"
	"    pushw %ds\n" // Save DS/ES
	"    pushw %es\n"
	"    pushw %ss\n" // Push SS 2x to load it into DS and ES
	"    pushw %ss\n"
	"    popw %ds\n"
	"    popw %es\n"
	"    call disk_handler\n" // Call handler
	"    popw %es\n" // Resroe DS/ES
	"    popw %ds\n"
	"    popa\n"
	"    iret\n"
);

void disk_handler() {
	byte status = inb(IDE_STATUS);
	print("DRQ=%u BUSY=%u DRDY=%u\n", (status & IDE_DRQ) != 0, (status & IDE_BUSY) != 0, (status & IDE_DRDY) != 0);

	PIC::eoi(IRQ_ATA_PRIMARY);
}

void ide_init() {
	registerInterruptHandler(IRQ_VECTOR_BASE + IRQ_ATA_PRIMARY, disk_isr);
	PIC::enable(IRQ_ATA_PRIMARY);
}