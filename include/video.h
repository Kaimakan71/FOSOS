/**
 * FOSOS VBE/VGA video driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <stdlib.h>
#include <stdarg.h>
#include <font.h>

// This info is returned by VBE and stored at physical address 0x2000
// Deprecated - no longer used
// Unused     - not used
// Reserved   - no meaning / for future expansion
typedef struct {
	word attributes;

	byte deprecated0;
	byte deprecated1;
	word deprecated2;

	UInt16 windowSize;
	UInt16 segmentA;
	UInt16 segmentB;

	dword deprecated3;
	UInt16 pitch;
	UInt16 width;
	UInt16 height;
	byte unused0;
	byte unused1;
	UInt8 planes;
	UInt8 bpp;
	byte deprecated4;
	UInt8 memoryModel;
	byte deprecated5;
	UInt8 imagePages;
	byte reserved0;
	byte redMask;
	UInt8 redPos;
	byte greenMask;
	UInt8 greenPos;
	byte blueMask;
	UInt8 bluePos;
	byte reservedMask;
	UInt8 reservedPos;
	byte directColorAttributes;
	UInt32 framebuffer; // Probably the most useful thing here
	UInt32 offscreenMem;
	UInt16 offscreenMem_size;
	byte reserved1[206];
} PACKED VBEModeInfo;

// VGA color pallette
// Not all colors are defined here (there are too many to fit)
typedef enum {
	// EGA 16-color pallette (row 0)
	EGA_Black    = 0x00,
	EGA_Blue     = 0x01,
	EGA_Green    = 0x02,
	EGA_Cyan     = 0x03,
	EGA_Red      = 0x04,
	EGA_Magenta  = 0x05,
	EGA_Brown    = 0x06,
	EGA_LGray    = 0x07,
	EGA_Gray     = 0x08,
	EGA_LBlue    = 0x09,
	EGA_LGreen   = 0x0a,
	EGA_LCyan    = 0x0b,
	EGA_LRed     = 0x0c,
	EGA_LMagenta = 0x0d,
	EGA_Yellow   = 0x0e,
	EGA_White    = 0x0f,

	// VGA gray colors, can be used as VGA_GRAY + <n>
	VGA_Gray = 0x10
} Color;

extern byte* videoMemory;
extern uint screenWidth;
extern uint screenHeight;
extern uint bitDepth;
extern byte textColor;
extern byte textColor;
extern uint cursorPos;
#define screenTop 0
#define screenLeft 0
#define screenBottom screenHeight
#define screenRight screenWidth
#define charWidth 8
#define charHeight 10

void fillRect(uint x, uint y, uint width, uint height, Color color);
void drawHLine(uint x, uint y, uint width, Color color);
void drawVLine(uint x, uint y, uint height, Color color);
void fillScreen(Color color);
void drawChar(char character, uint x, uint y, Color color);
void clearChar(uint pos);
void putChar(char character);
void printf(const char* fmt, ... );
void error(const char* msg);
void handleMouse(Int8 dx, Int8 dy);
void video_init();