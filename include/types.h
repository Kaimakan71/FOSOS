/**
 * Basic types
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define PURE __attribute__((pure))
#define MALLOC __attribute__((malloc))
#define PACKED __attribute__((packed))
#define NORETURN __attribute__((noreturn))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

typedef byte UInt8;
typedef word UInt16;
typedef dword UInt32;

typedef UInt8 bool;
#define true 1
#define false 0

#define LSW(x) ((UInt32)(x) & 0xFFFF)
#define MSW(x) (((UInt32)(x) >> 16) & 0xFFFF)
#define LSB(x) ((x) & 0xFF)
#define MSB(x) (((x)>>8) & 0xFF)