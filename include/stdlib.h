#pragma once

#include <types.h>

void memcpy(void*, const void*, UInt32);
void strcpy(char*, const char*);
int strcmp(char const*, const char*);
bool streq(const char* s1, const char* s2);
UInt32 strlen(const char*) PURE;
void *memset(void*, byte, UInt32);
int strspl(char *str, char delim, char* words[], int maxWords);