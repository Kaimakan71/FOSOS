/*
 * FOSOS keyboard driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <i386.h>
#include <io.h>
#include <vga.h>
#include <pic.h>
#include <shell.h>

#define IRQ_KEYBOARD 1

#define I8042_BUFFER   0x60
#define I8042_STATUS   0x64
#define SET_LEDS       0xED
#define DATA_AVAILABLE 0x01
#define I8042_ACK      0xFA

#define KBD_MOD_SHIFT 0x04
#define KBD_MOD_CTRL  0x02
#define KBD_MOD_ALT   0x01

namespace Keyboard {

enum class LED {
	ScrollLock = 1 << 0,
	NumLock = 1 << 1,
	CapsLock = 1 << 2,
};

void setLED(enum LED led);
void unsetLED(enum LED led);
void init();

};