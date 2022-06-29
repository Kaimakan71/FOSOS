/*
 * FOSOS standard library routines
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

void memcpy(void *dest, const void *src, UInt32 n) {
	byte* bdest = (byte*)dest;
	const byte* bsrc = (const byte*)src;
	for (; n; --n) *(bdest++) = *(bsrc++);
}

void strcpy(char* dest, const char *src) {
	while(*src) *(dest++) = *(src++);
	*(dest++) = '\0';
}

void* memset(void* dest, byte c, UInt32 n) {
	byte *bdest = (byte*)dest;
	for(; n; --n) *(bdest++) = c;
	return dest;
}

UInt32 strlen(const char* str) {
	UInt32 len = 0;
	while(*(str++)) ++len;
	return len;
}

int strcmp(const char* s1, const char* s2) {
	for(; *s1 == *s2; ++s1, ++s2) if(*s1 == 0) return 0;
	return *(const byte*)s1 < *(const byte*)s2 ? -1 : 1;
}

// Checks for string equality
// Does not care about which string is more/less, unlike strcmp()
bool streq(const char* s1, const char* s2) {
	for(; *s1 == *s2; ++s1, ++s2) if(*s1 == 0) return true;
	return false;
}

int strspl(char *str, char delim, char* words[], int maxWords) {
	int nextIndex = 0;

	while(*str != '\0') {
		while(*str == delim) *str++ = '\0';

		if(*str != '\0') {
			if(nextIndex < maxWords) words[nextIndex++] = str++;
			while(*str != '\0' && *str != delim) str++;
		}
	};

	return nextIndex;
}