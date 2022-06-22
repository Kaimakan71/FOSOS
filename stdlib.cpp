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