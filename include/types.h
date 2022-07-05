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

typedef UInt32 uint;

typedef char Int8;
typedef short Int16;
typedef int Int32;

typedef UInt8 bool;
#define true 1
#define false 0

#define NULL 0
#define nullptr (void*)NULL

#define divUp(a, b) (a + b - 1) / b

#define LSW(x) ((UInt32)(x) & 0xFFFF)
#define MSW(x) (((UInt32)(x) >> 16) & 0xFFFF)
#define LSB(x) ((x) & 0xFF)
#define MSB(x) (((x)>>8) & 0xFF)