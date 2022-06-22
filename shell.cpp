/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <shell.h>
#include <pit.h>

System system;

namespace Shell {

char inbuf[64];
UInt8 inbufPos;

void prompt() {
	print("\n");
	vga_setColor(0x0a);
	print(system.username);
	vga_setColor(0x07);
	print("@");
	vga_setColor(0x0f);
	print(system.hostname);
	vga_setColor(0x07);
	print(":");
	vga_setColor(0x0b);
	print(system.path);
	vga_setColor(0x07);
	print("> ");
}


void help() {
	print(
		"help     - prints this message\n"
		"pwd      - prints the current working directory\n"
		"clear    - clears the screen\n"
		"date     - prints the current date and time\n"
		"uptime   - prints the system's current uptime\n"
		"whoami   - prints your username\n"
		"hostname - prints the machine's hostname\n"
		"banner   - prints the FOSOS banner\n"
	);
}

// Print a banner in a bright color
void banner() {
	vga_setColor(0x0f);
	print("    dBBBBP dBBBBP.dBBBBP   dBBBBP.dBBBBP\n   dBP    dB'.BP BP       dB'.BP BP\n  dBBBP  dB'.BP  `BBBBb  dB'.BP  `BBBBb\n dBP    dB'.BP      dBP dB'.BP      dBP\ndBP    dBBBBP  dBBBBP' dBBBBP  dBBBBP'   v1.0\n");
	vga_setColor(0x07);
	print("Copyright (c) 2022, the FOSOS developers.\n\n");
}

void handleInput() {
	print("\n");

	// Run command
	if(strlen(inbuf) == 0); // Ignore empty lines
	else if(streq(inbuf, "help")) help();
	else if(streq(inbuf, "pwd")) print(system.path);
	else if(streq(inbuf, "clear")) clearScreen();
	else if(streq(inbuf, "date")) RTC::printTime();
	else if(streq(inbuf, "uptime")) {
		UInt32 seconds = system.uptime / TICKS_PER_SECOND;
		print("%um %us", seconds / 60, seconds % 60);
	} else if(streq(inbuf, "banner")) banner();
	else if(streq(inbuf, "whoami")) print(system.username);
	else if(streq(inbuf, "hostname")) print(system.hostname);
	else print("Unknown command '%s'", inbuf);

	prompt();

	// Empty the input buffer
	memset(inbuf, 0, sizeof(inbuf));
	inbufPos = 0;
}

void reset() {
	// Reset screen and color
	vga_setColor(0x07);
	clearScreen();

	// Print banner, time, and prompt
	banner();
	print("%uKiB base memory\nHello, %s! Current time is ", system.memory, system.username);
	RTC::printTime();
	print("\nType 'help' for a list of commands and use ctrl+c if you get stuck :-)\n");
	prompt();

	// Empty shell input buffer
	memset(inbuf, 0, sizeof(inbuf));
	inbufPos = 0;
}

void init() {
	// Set system defaults
	strcpy(system.username, "kai");
	strcpy(system.hostname, "native");
	strcpy(system.path, "/");
	system.uptime = 0;
	system.memory = (cmos_read(0x16) << 8) | cmos_read(0x15);

	reset();
}

};