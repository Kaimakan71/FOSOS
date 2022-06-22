/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <shell.h>
#include <pit.h>

System system;

char shell_inbuf[64];
UInt8 shell_inbufPos;

// Print a banner in a bright color
void shell_banner() {
	vga_setColor(0x0f);
	print("    dBBBBP dBBBBP.dBBBBP   dBBBBP.dBBBBP\n   dBP    dB'.BP BP       dB'.BP BP\n  dBBBP  dB'.BP  `BBBBb  dB'.BP  `BBBBb\n dBP    dB'.BP      dBP dB'.BP      dBP\ndBP    dBBBBP  dBBBBP' dBBBBP  dBBBBP'   v1.0\n");
	vga_setColor(0x07);
	print("Copyright (c) 2022, the FOSOS developers.\n\n");
}

void shell_date() {
	rtc_printTime();
	print("\n");
}

void shell_prompt() {
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

void shell_handleInput() {
	print("\n");

	// Run command
	if(strlen(shell_inbuf) == 0); // Ignore empty lines
	else if(streq(shell_inbuf, "help")) print("help     - prints this message\npwd      - prints the current working directory\nide      - a simple IDE ATA disk driver test\nclear    - clears the screen\ndate     - prints the current date and time\nwhoami   - prints your username\nhostname - prints the machine's hostname\nuptime   - prints the system's current uptime\nbanner   - prints the FOSOS banner\n");
	else if(streq(shell_inbuf, "pwd")) print("%s\n", system.path);
	else if(streq(shell_inbuf, "clear")) clearScreen();
	else if(streq(shell_inbuf, "ide")) ide_init();
	else if(streq(shell_inbuf, "date")) shell_date();
	else if(streq(shell_inbuf, "whoami")) print("%s\n", system.username);
	else if(streq(shell_inbuf, "hostname")) print("%s\n", system.hostname);
	else if(streq(shell_inbuf, "uptime")) {
		UInt32 seconds = system.uptime / TICKS_PER_SECOND;
		print(
			"%um %us\n",
			seconds / 60, // Seconds / 60, rounded down
			seconds % 60 // Remainder of seconds after /60
		);
	} else if(streq(shell_inbuf, "banner")) shell_banner();
	else print("Unknown command '%s'\n", shell_inbuf);
	// else if(streq(shell_inbuf, "gui")) gui_init();

	// Prompt
	shell_prompt();

	// Empty the input buffer
	memset(shell_inbuf, 0, sizeof(shell_inbuf));
	shell_inbufPos = 0;
}

void shell_reset() {
	// Reset screen and color
	vga_setColor(0x07);
	clearScreen();

	// Print banner, time, and prompt
	shell_banner();
	print("%uKiB base memory\nHello, %s! Current time is ", system.memory, system.username);
	shell_date();
	print("Type 'help' for a list of commands and use ctrl+c if you get stuck :-)\n");
	shell_prompt();

	// Empty shell input buffer
	memset(shell_inbuf, 0, sizeof(shell_inbuf));
	shell_inbufPos = 0;
}

void shell_init() {
	// Set system defaults
	strcpy(system.username, "kai");
	strcpy(system.hostname, "native");
	strcpy(system.path, "/");
	system.uptime = 0;
	system.memory = (cmos_read(0x16) << 8) | cmos_read(0x15);

	shell_reset();
}