/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <vga.h>
// #include <gui.h>
#include <stdlib.h>
#include <rtc.h>
#include <ide.h>

typedef struct {
	char username[32];
	char hostname[32];
	char path[256];
	UInt32 uptime;
	UInt16 memory; // In KiB
} System;
extern System system;

extern char shell_inbuf[64];
extern UInt8 shell_inbufPos;

void shell_banner();
void shell_prompt();
void shell_time();
void shell_handleInput();
void shell_reset();
void shell_init();