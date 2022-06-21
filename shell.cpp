/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <shell.h>

typedef struct {
	char username[32];
	char hostname[32];
} System;
System system;

char shell_inbuf[64];
UInt8 shell_inbufPos;

// Print a banner in a bright color
void banner() {
	vga_setColor(0x0f);
	print("    dBBBBP dBBBBP.dBBBBP   dBBBBP.dBBBBP\n   dBP    dB'.BP BP       dB'.BP BP\n  dBBBP  dB'.BP  `BBBBb  dB'.BP  `BBBBb\n dBP    dB'.BP      dBP dB'.BP      dBP\ndBP    dBBBBP  dBBBBP' dBBBBP  dBBBBP'   v1.0\n\n");
	vga_setColor(0x07);
}

void shell_setUsername(const char* name) {
	memset(&system.username, 0, sizeof(system.username));
	strcpy(system.username, name);
}

void shell_setHostname(const char* name) {
	memset(&system.hostname, 0, sizeof(system.hostname));
	strcpy(system.hostname, name);
}

void shell_prompt() {
	vga_setColor(0x0a);
	print(system.username);
	vga_setColor(0x07);
	print("@");
	vga_setColor(0x0f);
	print(system.hostname);
	vga_setColor(0x07);
	print("> ");
}

void shell_handleInput() {
	print("\n");

	// Run command
	if(streq(shell_inbuf, "help")) print("help   - prints this message\nclear  - clears the screen\nbanner - prints the FOSOS banner\ngui    - a broken WIP text UI demo\n");
	else if(streq(shell_inbuf, "clear")) clearScreen();
	else if(streq(shell_inbuf, "banner")) banner();
	else if(streq(shell_inbuf, "gui")) gui_init();
	else print("Unknown command '%s'\n", shell_inbuf);

	// Prompt
	shell_prompt();

	// Empty the input buffer
	memset(shell_inbuf, 0, sizeof(shell_inbuf));
	shell_inbufPos = 0;
}

void shell_init() {
	// Reset screen and color
	vga_setColor(0x07);
	clearScreen();

	// Set system defaults
	shell_setUsername("kai");
	shell_setHostname("native");

	// Print banner and prompt
	banner();
	print("Type 'help' for a list of commands and use ^c if you get stuck :-)\n");
	shell_prompt();

	// Empty shell input buffer
	memset(shell_inbuf, 0, sizeof(shell_inbuf));
	shell_inbufPos = 0;
}