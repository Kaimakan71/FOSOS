/**
 * Standard library functions
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

void memcpy(void *dest, const void *src, uint n) {
	byte* bdest = (byte*)dest;
	const byte* bsrc = (const byte*)src;
	for(; n; --n) *(bdest++) = *(bsrc++);
}

void strcpy(char* dest, const char *src) {
	while ((*dest++ = *src++) != '\0');
}

void* memset(void* dest, byte c, uint n) {
	byte *bdest = (byte *)dest;
	for(; n; --n) *(bdest++) = c;

	return dest;
}

char* strrchr(const char* str, int ch) {
	char *last = nullptr;
	char c;
	for(; (c = *str); ++str) {
		if(c == ch) last = (char*)str;
	}

	return last;
}

uint strlen(const char* str) {
	uint len = 0;
	while(*(str++)) ++len;

	return len;
}

int strcmp(const char *s1, const char *s2) {
	for(; *s1 == *s2; ++s1, ++s2) {
		if(*s1 == 0) return 0;
	}

	return *(const byte*)s1 < *(const byte*)s2 ? -1 : 1;
}

// Checks for string equality
// Does not care about which string is more/less, unlike strcmp()
bool streq(const char* s1, const char* s2) {
	for(; *s1 == *s2; s1++, s2++) if(*s1 == '\0') return true;
	return false;
}

char* strdup(const char *str) {
	uint len = strlen(str);
	char *s = (char*)malloc(len);
	memcpy(s, str, len);

	return s;
}

int memcmp(const void* v1, const void* v2, uint n) {
	const byte* s1 = (const byte*)v1;
	const byte* s2 = (const byte*)v2;
	while(n-- > 0) {
		if(*s1++ != *s2++) return s1[-1] < s2[-1] ? -1 : 1;
	}

	return 0;
}

// Splits a string on delim
uint strspl(char *str, char delim, char* words[], uint maxWords) {
	uint nextIndex = 0;

	while(*str != '\0') {
		while(*str == delim) *str++ = '\0';

		if(*str != '\0') {
			if(nextIndex < maxWords) words[nextIndex++] = str++;
			while(*str != '\0' && *str != delim) str++;
		}
	};

	return nextIndex;
}