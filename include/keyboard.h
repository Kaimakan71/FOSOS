/**
 * FOSOS PS2-compatible mouse driver
 * Currently US-only (plans for more later)
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <cputable.h>
#include <ports.h>
#include <video.h>
#include <i8259.h>

#define IRQ_KEYBOARD 1

#define I8042_BUFFER   0x60
#define I8042_STATUS   0x64
#define SET_LEDS       0xED
#define DATA_AVAILABLE 0x01
#define I8042_ACK      0xFA

#define KBD_MOD_SHIFT 0x04
#define KBD_MOD_CTRL  0x02
#define KBD_MOD_ALT   0x01

void kbd_init();