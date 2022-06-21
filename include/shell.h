/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <vga.h>
#include <gui.h>
#include <stdlib.h>
#include <rtc.h>

extern char shell_inbuf[64];
extern UInt8 shell_inbufPos;

void banner();
void shell_handleInput();
void shell_init();