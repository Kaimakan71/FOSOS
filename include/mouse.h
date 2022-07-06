/**
 * FOSOS PS2 mouse driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <ports.h>
#include <video.h>
#include <cputable.h>
#include <i8259.h>
#include <gui.h>

extern int mouseX;
extern int mouseY;

#define IRQ_PS2MOUSE 12

void drawCursor();
void mouse_init();