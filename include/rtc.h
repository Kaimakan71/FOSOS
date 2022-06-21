/*
 * FOSOS RTC driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <io.h>
#include <vga.h>

void rtc_printTime();
void rtc_init();