/**
 * FOSOS memory manager
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <video.h>
#include <cputable.h>
#include <stdlib.h>

void* malloc(uint amount);
void free(void* ptr);
void memory_init();