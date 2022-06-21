#pragma once

#define PUBLIC
#define PRIVATE static

#define NORETURN __attribute__((noreturn))
#define PURE __attribute__((pure))
#define MALLOC __attribute__((malloc))
#define PACKED __attribute__((packed))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

typedef byte UInt8;
typedef word UInt16;
typedef dword UInt32;

typedef UInt32 Time;

#define true 1
#define false 0

#define HANG asm volatile("cli; hlt");
#define LSW(x) ((dword)(x) & 0xFFFF)
#define MSW(x) (((dword)(x) >> 16) & 0xFFFF)
#define LSB(x) ((x) & 0xFF)
#define MSB(x) (((x)>>8) & 0xFF)