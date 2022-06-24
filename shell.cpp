/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <shell.h>

System system;

namespace Shell {

char inbuf[256];
UInt8 inbufPos;

void prompt() {
	VGA::setColor(0x0a);
	print(system.username);
	VGA::setColor(0x07);
	print("@");
	VGA::setColor(0x0f);
	print(system.hostname);
	VGA::setColor(0x07);
	print(":");
	VGA::setColor(0x0b);
	print(system.path);
	VGA::setColor(0x07);
	print("> ");
}

int $help(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	print(
		"help     - prints this message\n"
		"initfs   - initializes the experimental file system\n"
		"pwd      - prints the current working directory\n"
		"realpath - get the full path of a file\n"
		"clear    - clears the screen\n"
		"date     - prints the current date and time\n"
		"uptime   - prints the system's current uptime\n"
		"whoami   - prints your username\n"
		"hostname - prints the machine's hostname\n"
		"banner   - prints the FOSOS banner"
	);
	return 0;
}

int $pwd(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	print(system.path);
	return 0;
}

int $realpath(int argc, char* argv[]) {
	REQUIRE_EXACT_ARGS(2);

	// If not full path, add current directory
	if(*argv[1] != '/') print(system.path);
	// If the current directory is not root, add a /
	if(strlen(system.path) > 1) print("/");
	print(argv[1]);
	return 0;
}

int $clear(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	VGA::clearScreen();
	return 0;
}

int $date(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	RTC::printTime();
	return 0;
}

int $uptime(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	UInt32 seconds = system.uptime / TICKS_PER_SECOND;
	print("%um %us", seconds / 60, seconds % 60);
	return 0;
}

int $whoami(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	print(system.username);
	return 0;
}

int $hostname(int argc, char* argv[]) {
	DISALLOW_ARGS(2);

	if(argc > 1) {
		strcpy(system.hostname, argv[1]);
		print("Hostname set to ");
	}

	print(system.hostname);
	return 0;
}

// Print a banner in a bright color
int $banner(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	VGA::setColor(0x0f);
	print("    dBBBBP dBBBBP.dBBBBP   dBBBBP.dBBBBP\n   dBP    dB'.BP BP       dB'.BP BP\n  dBBBP  dB'.BP  `BBBBb  dB'.BP  `BBBBb\n dBP    dB'.BP      dBP dB'.BP      dBP\ndBP    dBBBBP  dBBBBP' dBBBBP  dBBBBP'   v1.0\n");
	VGA::setColor(0x07);
	print("Copyright (c) 2022, the FOSOS developers.\n\n");
	
	return 0;
}

int invoke(int argc, char* argv[]) {
	if(streq(argv[0], "help")) return $help(argc, argv);
	if(streq(argv[0], "initfs")) {
		VGA::clearScreen(); // Clear screen, there will be a lot of printing
		FS::init();
		return 0;
	}
	if(streq(argv[0], "pwd")) return $pwd(argc, argv);
	if(streq(argv[0], "realpath")) return $realpath(argc, argv);
	if(streq(argv[0], "clear")) return $clear(argc, argv);
	if(streq(argv[0], "date")) return $date(argc, argv);
	if(streq(argv[0], "uptime")) return $uptime(argc, argv);
	if(streq(argv[0], "banner")) return $banner(argc, argv);
	if(streq(argv[0], "whoami")) return $whoami(argc, argv);
	if(streq(argv[0], "hostname")) return $hostname(argc, argv);

	print("Unknown command '%s'", argv[0]);
	return -1;
}

// Invoke with no arguments
int run(const char* name) {
	char* argv[1] = { const_cast<char*>(name) };
	return invoke(1, argv);
}

void handleInput() {
	print("\n");

	char* argv[128];
	int argc = strspl(inbuf, ' ', argv, sizeof(argv) / sizeof(argv[0]));

	// Ignore empty lines
	if(strlen(argv[0]) > 0) {
		// Run command
		invoke(argc, argv);
		print("\n");
	}

	// Prompt for next command
	prompt();

	// Empty the input buffer
	memset(inbuf, 0, sizeof(inbuf));
	inbufPos = 0;
}

void reset() {
	// Reset screen and color
	VGA::setColor(0x07);
	VGA::clearScreen();

	// Print banner, time, and prompt
	run("banner");
	print("%uKiB base memory\nHello, %s! Current time is ", system.memory, system.username);
	run("date");
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