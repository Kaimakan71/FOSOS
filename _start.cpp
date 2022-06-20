/*
 * _start - kernel entry point
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

extern void init();

extern "C" void _start() {
	init();
	asm volatile("cli; hlt");
}