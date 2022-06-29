/**
 * i8259 Programmable Interrupt Controller driver for FOSOS
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <ports.h>
#include <i386.h>

#define SLAVE_INDEX 2
#define PIC0_CMD 0x20
#define PIC0_DAT 0x21
#define PIC1_CMD 0xA0
#define PIC1_DAT 0xA1
#define PIC0_VECTOR_BASE (IRQ_VECTOR_BASE)
#define PIC1_VECTOR_BASE (IRQ_VECTOR_BASE + 0x08)

void pic_enable(UInt8 vector);
void pic_disable(UInt8 vector);
void pic_eoi(UInt8 vector);
void pic_dumpRegs();
void pic_init();