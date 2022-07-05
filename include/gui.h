/**
 * FOSOS GUI routines
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <ports.h>
#include <video.h>
#include <mouse.h>

typedef struct {
	Color primary;
	Color secondary;
	Color tertiary;
	int taskbarHeight;
	int menuHeight;
} Theme;

extern Theme theme;
void createWindow(const char* name, int x, int y, int width, int height, Color background);
void gui_init();