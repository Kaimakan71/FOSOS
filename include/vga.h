/*
 * VGA text mode driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <io.h>
#include <stdlib.h>
#include <stdarg.h>

#define VGA_WIDTH 80
#define VGA_MIDDLE_X 40
#define VGA_HEIGHT 25
#define VGA_MIDDLE_Y 12
#define VGA_CHARS (VGA_WIDTH * VGA_HEIGHT)

const char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void putChar(char character);
void putCharAt(char character, UInt8 x, UInt8 y);
void fillRect(UInt8 x, UInt8 y, UInt8 width, UInt8 height, byte color);
void clearChar(UInt16 index);
void print(const char *fmt, ...);

namespace VGA {

extern volatile UInt16 cursor;
byte getColor();
void setColor(byte);
UInt16 getCursor();
void setCursor(UInt16);
void setCursor(UInt8 x, UInt8 y);
void putCharCursor(char character);
void clearScreen();

};