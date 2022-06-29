#pragma once

#include <types.h>
#include <ports.h>
#include <stdlib.h>
#include <stdarg.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CHARS (VGA_WIDTH * VGA_HEIGHT)

extern volatile UInt16 vga_cursor;

byte vga_getColor();
void vga_setColor(UInt8 fg, UInt8 bg);

UInt16 vga_getCursor();
void vga_updateCursor();

void putChar(char character);
void clearChar(UInt16 index);
void putCharAt(char character, UInt8 x, UInt8 y);

void printUInt(UInt32 number);
void printInt(int number);
void printHex(UInt32 number, UInt8 fields);
void printf(const char* fmt, ... );

void clearScreen();
void printCenter(const char* string);
void panic(const char* message);
void error(const char* message);

void vga_init();