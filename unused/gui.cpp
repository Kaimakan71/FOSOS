/*
 * FOSOS GUI functions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gui.h>

#define MESSAGEBOX_HEIGHT 5
#define MESSAGEBOX_MIDDLE (MESSAGEBOX_HEIGHT / 2)

PRIVATE void drawWindow(UInt8 width, UInt8 height) {
	// Center on screen
	UInt8 x = VGA_MIDDLE_X - (width / 2);
	UInt8 y = VGA_MIDDLE_Y - (height / 2);

	// Shadow
	fillRect(x, y, width, height, 0x00);
	// Window background
	fillRect(x - 1, y - 1, width, height, 0xf0);
}

PRIVATE void messageBox(const char* title, const char* text) {
	UInt8 width = strlen(text) + 6;

	// Center on screen
	UInt8 x = VGA_MIDDLE_X - (width / 2);
	UInt8 y = VGA_MIDDLE_Y - MESSAGEBOX_MIDDLE;

	VGA::setColor(0xf0);

	// Shadow
	fillRect(x, y, width , MESSAGEBOX_HEIGHT, 0x00);
	// Window background
	fillRect(x - 1, y - 1, width, MESSAGEBOX_HEIGHT, 0xf0);

	// Title
	VGA::setCursor(x, y - 1);
	print(title);

	// Message text
	VGA::setCursor(x + 2, y + 1);
	print(text);
}

PUBLIC void gui_init() {
	VGA::setColor(0x90);
	clearScreen();
	messageBox("About FOSOS", "FOSOS version 1.0");
}