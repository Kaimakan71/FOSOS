/**
 * i386 CPU driver for FOSOS
 * For manipulating CPU tables and exceptions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <assert.h>
#include <i8259.h>
#include <video.h>

typedef struct {
	UInt16 limit;
	void* base;
} PACKED DTR;

typedef union {
	struct {
		UInt16 limit_lo;
		UInt16 base_lo;
		byte base_hi;
		byte type: 4;
		byte descriptorType: 1;
		byte dpl: 2;
		byte present: 1;
		byte limit_hi: 4;
		byte: 1;
		byte zero: 1;
		byte operationSize: 1;
		byte granularity: 1;
		byte base_hi2;
	};
	struct {
		UInt32 low;
		UInt32 high;
	};
} PACKED Descriptor;

#define IRQHANDLER(i) \
	extern void handleIRQ ## i (); \
	extern void isr ## i (); \
	asm( \
	".globl isr" #i "\n" \
	"isr" #i ":\n" \
	"    pusha\n" \
	"    pushw %ds\n" \
	"    pushw %es\n" \
	"    pushw %fs\n" \
	"    pushw %gs\n" \
	"    mov %esp, %eax\n" \
	"    call handleIRQ" #i "\n" \
	"    popw %gs\n" \
	"    popw %fs\n" \
	"    popw %es\n" \
	"    popw %ds\n" \
	"    popa\n" \
	"    iret\n" \
);


Descriptor* getGDTEntry(UInt16 selector);
UInt16 allocateGDTEntry();
void writeGDTEntry(UInt16 selector, Descriptor desc);
void setDescriptorBase(Descriptor* desc, void* base);
void setDescriptorLimit(Descriptor* desc, UInt32 limit);
void flushGDT();
void gdt_init();
void registerInterruptHandler(UInt8 vector, void (*f)());
void registerUserInterruptHandler(UInt8 vector, void (*f)());
#define registerIRQHandler(i, h) registerInterruptHandler(IRQ_VECTOR_BASE + i, h);
void idt_init();