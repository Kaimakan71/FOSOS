#pragma once

#include <types.h>
#include <assert.h>

void memcpy(void*, const void*, UInt32);
void strcpy(char*, const char*);
int strcmp(char const*, const char*);
UInt32 strlen(const char*) PURE;
void *memset(void*, byte, UInt32);
char *strdup(const char*);