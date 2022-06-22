/*
 * Basic PIC controller
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <i386.h>
#include <io.h>

#define SLAVE_INDEX 2
#define PIC0_CMD 0x20
#define PIC0_DAT 0x21
#define PIC1_CMD 0xA0
#define PIC1_DAT 0xA1

namespace PIC {

void enable(UInt8 vector);
void disable(UInt8 vector);
void eoi(UInt8 vector);
void init();

};