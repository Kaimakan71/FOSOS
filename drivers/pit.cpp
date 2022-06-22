/*
 * i8253 PIT driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <pit.h>

namespace PIT {

extern "C" void tick_isr();
extern "C" void handle_clock();

asm(
	".globl tick_isr\n"
	"tick_isr:\n"
	"    pusha\n"
	"    pushw %ds\n"
	"    pushw %es\n"
	"    pushw %fs\n"
	"    pushw %gs\n"
	"    mov %esp, %eax\n"
	"    call handle_clock\n"
	"    popw %gs\n"
	"    popw %fs\n"
	"    popw %es\n"
	"    popw %ds\n"
	"    popa\n"
	"    iret\n"
);

void handle_clock() {
	system.uptime++;
	PIC::eoi(IRQ_TIMER);
}

void init() {
	outb(PIT_CMD, TIMER0_SELECT | WRITE_WORD | TIMER_MODE_SQUARE_WAVE);

	UInt16 reload = (BASE_FREQ / TICKS_PER_SECOND);
	outb(TIMER0_CMD, LSB(reload));
	outb(TIMER0_CMD, MSB(reload));

	registerInterruptHandler(IRQ_VECTOR_BASE + IRQ_TIMER, tick_isr);
	PIC::enable(IRQ_TIMER);
}

};