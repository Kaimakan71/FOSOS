/*
 * Basic PIC controller
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <i386.h>
#include <io.h>

void pic_enable(UInt8 vector);
void pic_disable(UInt8 vector);
void pic_eoi(UInt8 vector);
void pic_init();

class IRQHandlerScope {
public:
	explicit IRQHandlerScope(byte irq) : m_irq(irq) { }
	~IRQHandlerScope() { pic_eoi(m_irq); }
private:
	byte m_irq { 0 };
};