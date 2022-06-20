/*
 * i386-specific utilities
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <types.h>
#include <vga.h>
#include <assert.h>

union Descriptor {
	struct {
		UInt16 limit_lo;
		UInt16 base_lo;
		byte base_hi;
		byte type: 4;
		byte descriptor_type: 1;
		byte dpl: 2;
		byte segment_present: 1;
		byte limit_hi: 4;
		byte: 1;
		byte zero: 1;
		byte operation_size: 1;
		byte granularity: 1;
		byte base_hi2;
	};
	struct {
		UInt32 low;
		UInt32 high;
	};

	enum Type {
		Invalid = 0,
		AvailableTSS_16bit = 0x1,
		LDT = 0x2,
		BusyTSS_16bit = 0x3,
		CallGate_16bit = 0x4,
		TaskGate = 0x5,
		InterruptGate_16bit = 0x6,
		TrapGate_16bit = 0x7,
		AvailableTSS_32bit = 0x9,
		BusyTSS_32bit = 0xb,
		CallGate_32bit = 0xc,
		InterruptGate_32bit = 0xe,
		TrapGate_32bit = 0xf,
	};

	void setBase(void* b) {
		base_lo = (UInt32)(b) & 0xffff;
		base_hi = ((UInt32)(b) >> 16) & 0xff;
		base_hi2 = ((UInt32)(b) >> 24) & 0xff;
	}

	void setLimit(UInt32 l) {
		limit_lo = (UInt32)l & 0xffff;
		limit_hi = ((UInt32)l >> 16) & 0xff;
	}
} PACKED;

void gdt_init();
void idt_init();
void registerInterruptHandler(UInt8 vector, void (*f)());
void registerUserCallableInterruptHandler(UInt8 vector, void (*f)());
void flushIDT();
void flushGDT();
void loadTaskRegister(UInt16 selector);
word allocateGDTEntry();
Descriptor& getGDTEntry(UInt16 selector);
void writeGDTEntry(UInt16 selector, Descriptor&);

#define disableInterrupts() asm volatile("cli");
#define enableInterrupts() asm volatile("sti");

#define IRQ_VECTOR_BASE 0x50