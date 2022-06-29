#pragma once

#include <types.h>
#include <vga.h>

#define hang() asm volatile("cli; hlt");
#define ASSERT(x) if(!(x)) { panic("ASSERTION FAILED"); print(#x " - %s:%u (%s)", __FILE__, __LINE__, __PRETTY_FUNCTION__); hang(); }
#define ASSERT_UNREACHABLE() panic("ASSERT UNREACHABLE FAILED"); print("%s:%u (%s)", __FILE__, __LINE__, __PRETTY_FUNCTION__); hang();