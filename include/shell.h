/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <ports.h>
#include <pit.h>
#include <rtc.h>
#include <vga.h>

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

extern char shell_inbuf[256];
extern UInt8 shell_inbufPos;

void shell_handleInput();
void shell_reset();
void shell_init();