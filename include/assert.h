/*
 * Basic assertions and debugging utilities
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <vga.h>

#define ASSERT(x) if(!(x)) { VGA::setColor(0x4f); print("ASSERTION " #x " FAILED IN %s:%u (%s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__); hang(); }
#define ASSERT_UNREACHABLE() ASSERT(false)