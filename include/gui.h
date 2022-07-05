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

typedef struct {
	Color primary;
	Color secondary;
	Color tertiary;
	int taskbarHeight;
	int menuHeight;
} Theme;

void gui_init();