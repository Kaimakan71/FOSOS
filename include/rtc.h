/**
 * FOSOS real-time clock driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <ports.h>
#include <vga.h>

void rtc_printDate();
void rtc_init();