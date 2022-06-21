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

enum class KeyboardLED {
	ScrollLock = 1 << 0,
	NumLock = 1 << 1,
	CapsLock = 1 << 2,
};

void keyboard_setLED(enum KeyboardLED led);
void keyboard_unsetLED(enum KeyboardLED led);
void keyboard_handleInterrupt();
void keyboard_init();