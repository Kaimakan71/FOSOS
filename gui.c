/**
 * FOSOS GUI routines
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gui.h>

Theme theme;
uint windowCount = 0;
int currentWindowId = -1;
Window** windows;

#define clearScreen() fillScreen(theme.secondary);

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

void drawWindow(Window* window) {
	fillRect(window->x, window->y, window->width, window->height, theme.primary);

	// Top/left border
	drawHLine(window->x + 1, window->y + 1, window->width - 2, theme.tertiary);
	drawVLine(window->x + 1, window->y + 1, window->height - 2, theme.tertiary);

	// Bottom/right border
	drawHLine(window->x + 1, window->y + window->height - 2, window->width - 2, theme.tertiary);
	drawVLine(window->x + window->width - 2, window->y + 1, window->height - 2, theme.tertiary);

	// Titlebar
	fillRect(window->x + 3, window->y + 3, window->width - 6, theme.menuHeight, EGA_LBlue);
	drawString(window->name, window->x + 7, window->y + 7, EGA_White);
	
	// Background
	fillRect(window->x + 3, window->y + theme.menuHeight + 4, window->width - 6, window->height - theme.menuHeight - 6, window->background);

	// Make sure the mouse wasn't covered
	drawCursor();
}

Window* createWindow(const char* name, uint x, uint y, uint width, uint height, Color background) {
	currentWindowId = windowCount++;

	debugf("Creating window '%s' with id %d\n", name, currentWindowId);

	Window* window = windows[currentWindowId];
	window->name = (char*)name;
	window->x = x;
	window->y = y;
	window->width = width;
	window->height = height;
	window->background = background;

	drawWindow(window);

	return window;
}

void handleDepress() {
	debugf("Mouse button depressed at (%d, %d)\n", mouseX, mouseY);
}

void handleRelease() {
	debugf("Mouse button released at (%d, %d)\n", mouseX, mouseY);
}

void gui_init() {
	video_init();

	theme.primary = VGA_Gray + 3;
	theme.secondary = VGA_Gray + 1;
	theme.tertiary = VGA_Gray + 6;
	theme.taskbarHeight = screenHeight * 0.05;
	theme.menuHeight = screenHeight * 0.04;

	clearScreen();

	windows = malloc(sizeof(Window*) * 256);

	mouse_init();
	// Draw the cursor at the current position
	drawCursor();
}