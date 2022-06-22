/*
 * i8253 PIT driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <io.h>
#include <pic.h>
#include <shell.h>

#define IRQ_TIMER 0

#define TIMER0_CMD 0x40
#define TIMER1_CMD 0x41
#define TIMER2_CMD 0x42
#define PIT_CMD 0x43
#define WRITE_WORD 0x30

#define TIMER0_SELECT 0x00
#define TIMER1_SELECT 0x40
#define TIMER2_SELECT 0x80

#define TIMER_MODE_COUNTDOWN 0x00
#define TIMER_MODE_ONESHOT 0x02
#define TIMER_MODE_RATE 0x04
#define TIMER_MODE_SQUARE_WAVE 0x06
#define BASE_FREQ 1193182
#define TICKS_PER_SECOND 600

namespace PIT {

void init();

};