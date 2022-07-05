/**
 * FOSOS GUI routines
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gui.h>

Theme theme;

#define clearScreen() fillScreen(theme.secondary);

void drawTaskbar() {
	fillRect(0, screenBottom - theme.taskbarHeight, screenWidth, theme.taskbarHeight, theme.primary);
	drawHLine(0, screenBottom - theme.taskbarHeight + 1, screenWidth, theme.tertiary);
}

void drawMenubar() {
	fillRect(0, 0, screenWidth, theme.menuHeight, theme.primary);
	drawHLine(0, theme.menuHeight - 2, screenWidth, theme.tertiary);
}

void drawString(const char* str, int x, int y, Color color) {
	int i = 0;
	while(*str) {
		drawChar(*str++, x + (i * charWidth), y, color);
		i++;
	}
}

void drawButton(const char* text, int x, int y, int height) {
	int width = (strlen(text) * charWidth) + 8;

	fillRect(x + 1, y + 1, width - 3, height - 3, theme.primary);

	// Top/left border
	drawHLine(x, y, width - 1, theme.tertiary);
	drawVLine(x, y + 1, height - 1, theme.tertiary);

	// Inner bottom/right border
	drawHLine(x + 1, y + height - 2, width - 2, theme.secondary);
	drawVLine(x + width - 2, y + 1, height - 2, theme.secondary);

	// Outer bottom/right border
	drawHLine(x, y + height - 1, width, VGA_Gray);
	drawVLine(x + width - 1, y, height, VGA_Gray);

	drawString(text, x + 4, y + 4, EGA_White);
}

void drawTaskbarButton(const char* text, int x) {
	drawButton(text, x + 2, screenHeight - theme.taskbarHeight + 4, theme.taskbarHeight - 6);
}

void drawWindow(const char* title, int x, int y, int width, int height, Color background) {
	fillRect(x, y, width, height, theme.primary);

	// Top/left border
	drawHLine(x + 1, y + 1, width - 2, theme.tertiary);
	drawVLine(x + 1, y + 1, height - 2, theme.tertiary);

	// Bottom/right border
	drawHLine(x + 1, y + height - 2, width - 2, theme.tertiary);
	drawVLine(x + width - 2, y + 1, height - 2, theme.tertiary);

	// Titlebar
	fillRect(x + 3, y + 3, width - 6, theme.menuHeight, EGA_LBlue);
	drawString(title, x + 7, y + 7, EGA_White);

	// Close button
	drawButton("X", x + width - 20, y + 4, 17);
	
	// Background
	fillRect(x + 3, y + theme.menuHeight + 4, width - 6, height - theme.menuHeight - 6, background);
}

void createApplication(const char* name, int x, int y, int width, int height, Color background) {
	drawTaskbarButton(name, 0);
	drawWindow(name, x, y, width, height, background);
}

void gui_init() {
	video_init();

	theme.primary = VGA_Gray + 3;
	theme.secondary = VGA_Gray + 1;
	theme.tertiary = VGA_Gray + 6;
	theme.taskbarHeight = screenHeight * 0.05;
	theme.menuHeight = screenHeight * 0.04;

	clearScreen();
	drawTaskbar();
	drawMenubar();

	createApplication("Shell?", 50, 50, 320, 200, VGA_Gray);
}