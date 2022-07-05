/**
 * Standard library functions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <memory.h>

void memcpy(void* dest, const void* src, uint n);
void strcpy(char* dest, const char* src);
void* memset(void* dest, byte c, uint n);
char* strrchr(const char* str, int ch);
uint strlen(const char* str);
int strcmp(const char* s1, const char* s2);
bool streq(const char* s1, const char* s2);
char* strdup(const char* str);
int memcmp(const void* v1, const void* v2, uint n);
uint strspl(char *str, char delim, char* words[], uint maxWords);