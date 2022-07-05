/**
 * i8259 Programmable Interrupt Controller driver for FOSOS
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <ports.h>
#include <video.h>

#define SLAVE_INDEX 2
#define PIC0_CMD 0x20
#define PIC0_DAT 0x21
#define PIC1_CMD 0xA0
#define PIC1_DAT 0xA1
#define IRQ_VECTOR_BASE 0x20
#define PIC0_VECTOR_BASE (IRQ_VECTOR_BASE)
#define PIC1_VECTOR_BASE (IRQ_VECTOR_BASE + 0x08)

void enableIRQ(UInt8 vector);
void disableIRQ(UInt8 vector);
void exitIRQHandler(UInt8 vector);
#define exitIRQHandler(vector) if(vector & 8) { outb(PIC1_CMD, 0x20); } outb(PIC0_CMD, 0x20);

void pic_init();