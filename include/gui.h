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

typedef struct {
	char* name;
	uint x;
	uint y;
	uint width;
	uint height;
	Color background;
} Window;

extern Theme theme;
Window* createWindow(const char* name, uint x, uint y, uint width, uint height, Color background);
void handleDepress();
void handleRelease();
void gui_init();