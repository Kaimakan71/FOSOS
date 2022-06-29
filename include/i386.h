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
#include <pic.h>
#include <vga.h>

#define IRQ_VECTOR_BASE 0x20

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

void gdt_init();
void registerInterruptHandler(UInt8 vector, void (*f)());
void registerUserInterruptHandler(UInt8 vector, void (*f)());
void idt_init();

#define enableInterrupts() asm volatile("sti");