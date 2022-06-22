/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <vga.h>
#include <stdlib.h>
#include <rtc.h>

typedef struct {
	char username[32];
	char hostname[32];
	char path[256];
	UInt32 uptime;
	UInt16 memory; // In KiB
} System;
extern System system;

namespace Shell {

extern char inbuf[64];
extern UInt8 inbufPos;

void handleInput();
void reset();
void init();

};