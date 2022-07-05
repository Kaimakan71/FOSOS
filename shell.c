/*
 * FOSOS shell
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <shell.h>

System system;

char* shell_inbuf;
uint shell_inbufPos;

void shell_prompt() {
	textColor = EGA_LGreen;
	printf(system.username);
	textColor = EGA_LGray;
	putChar('@');
	textColor = EGA_White;
	printf(system.hostname);
	textColor = EGA_LGray;
	putChar(':');
	textColor = EGA_LCyan;
	printf(system.path);
	textColor = EGA_LGray;
	putChar('>');
	putChar(' ');
}

int $help(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	printf(
		"help     - prints this message\n"
		"uname    - prints system information\n"
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

int $uname(int argc, char* argv[]) {
	for(int i = 1; i < argc; i++) {
		if(streq(argv[i], "-s")) printf(system.sysname);
		else if(streq(argv[i], "-n")) printf(system.hostname);
		else if(streq(argv[i], "-r")) printf(system.release);
		else if(streq(argv[i], "-m")) printf(system.machine);
		else if(streq(argv[i], "-a")) {
			printf(system.sysname);
			putChar(' ');
			printf(system.hostname);
			putChar(' ');
			printf(system.release);
			putChar(' ');
			printf(system.machine);
		}
		putChar(' ');
	}

	return 0;
}

int $pwd(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	printf(system.path);
	return 0;
}

int $realpath(int argc, char* argv[]) {
	REQUIRE_EXACT_ARGS(2);

	// If not full path, add current directory
	if(*argv[1] != '/') printf(system.path);
	// If the current directory is not root, add a /
	if(strlen(system.path) > 1) putChar('/');
	printf(argv[1]);
	return 0;
}

int $clear(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	clearScreen();
	return 0;
}

int $date(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	rtc_printDate();

	return 0;
}

int $uptime(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	UInt32 seconds = system.uptime / TICKS_PER_SECOND;
	printf("%um %us", seconds / 60, seconds % 60);
	return 0;
}

int $whoami(int argc, char* argv[]) {
	DISALLOW_ARGS(1);

	printf(system.username);
	return 0;
}

int $hostname(int argc, char* argv[]) {
	DISALLOW_ARGS(2);

	if(argc > 1) {
		strcpy(system.hostname, argv[1]);
		printf("Hostname set to ");
	}

	printf(system.hostname);
	return 0;
}

// Print a banner in a bright color
int $banner() {
	textColor = EGA_White;
	printf("    dBBBBP dBBBBP.dBBBBP   dBBBBP.dBBBBP\n   dBP    dB'.BP BP       dB'.BP BP\n  dBBBP  dB'.BP  `BBBBb  dB'.BP  `BBBBb\n dBP    dB'.BP      dBP dB'.BP      dBP\ndBP    dBBBBP  dBBBBP' dBBBBP  dBBBBP'   v1.0\n");
	textColor = EGA_LGray;
	printf("Copyright (c) 2022, the FOSOS developers.\n\n");

	return 0;
}

int invoke(int argc, char* argv[]) {
	if(streq(argv[0], "help")) return $help(argc, argv);
	if(streq(argv[0], "uname")) return $uname(argc, argv);
	if(streq(argv[0], "pwd")) return $pwd(argc, argv);
	if(streq(argv[0], "realpath")) return $realpath(argc, argv);
	if(streq(argv[0], "clear")) return $clear(argc, argv);
	if(streq(argv[0], "date")) return $date(argc, argv);
	if(streq(argv[0], "uptime")) return $uptime(argc, argv);
	if(streq(argv[0], "banner")) return $banner(argc, argv);
	if(streq(argv[0], "whoami")) return $whoami(argc, argv);
	if(streq(argv[0], "hostname")) return $hostname(argc, argv);

	printf("Unknown command '%s'", argv[0]);
	return -1;
}

// Invoke with no arguments
int run(const char* name) {
	char argv0[strlen(name)];
	strcpy(argv0, name);
	char* argv[1] = { argv0 };
	return invoke(1, argv);
}

void shell_handleInput() {
	putChar('\n');

	char* argv[128];
	int argc = strspl(shell_inbuf, ' ', argv, sizeof(argv) / sizeof(argv[0]));

	// Ignore empty lines
	if(strlen(shell_inbuf) > 0) {
		// Run command
		invoke(argc, argv);
		putChar('\n');
	}

	// Prompt for next command
	shell_prompt();

	// Empty the input buffer
	memset(shell_inbuf, 0, INBUF_SIZE);
	shell_inbufPos = 0;
}

void shell_reset() {
	// Reset screen and color
	textColor = EGA_LGray;
	clearScreen();

	// Print banner, time, and prompt
	run("banner");
	printf("%uKiB base memory\nHello, %s! Current time is ", system.memory, system.username);
	run("date");
	printf("\nType 'help' for a list of commands and use ctrl+c if you get stuck :~)\n");
	shell_prompt();

	// Empty shell input buffer
	memset(shell_inbuf, 0, INBUF_SIZE);
	shell_inbufPos = 0;
}

void shell_init() {
	shell_inbuf = (char*)malloc(INBUF_SIZE);

	// Set system defaults
	strcpy(system.username, "kai");
	strcpy(system.sysname, "FOSOS");
	strcpy(system.hostname, "native");
	strcpy(system.release, "1.0.0-dev");
	strcpy(system.machine, "i386");
	strcpy(system.path, "/");

	system.uptime = 0;
	system.memory = (cmos_read(0x16) << 8) | cmos_read(0x15);

	shell_reset();
}