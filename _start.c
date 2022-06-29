/**
 * FOSOS kernel entry
 * Starts the kernel by calling init
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

extern void init();

extern void _start() {
	init();
	asm volatile("cli; hlt");
}