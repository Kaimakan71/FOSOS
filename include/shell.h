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
#include <pit.h>
#include <ide.h>

typedef struct {
	char username[32];
	char hostname[32];
	char path[256];
	UInt32 uptime;
	UInt16 memory; // In KiB
} System;
extern System system;

#define DISALLOW_ARGS(i) if(argc > i) { error("too many arguments in invocation"); return 1; }
#define REQUIRE_ARGS(i) if(argc < i) { error("not enough arguments in invocation"); return 1; }
#define REQUIRE_EXACT_ARGS(i) if(argc != i) { error("incorrect number of arguments in invocation"); return 1; }

namespace Shell {

extern char inbuf[256];
extern UInt8 inbufPos;

void handleInput();
void reset();
void init();

};