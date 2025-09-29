#pragma once
#ifndef STRING_H
#define STRING_H

#include <stddef.h>

extern int memcmp(const void* a, const void* b, size_t sz);
extern void* memcpy(void* dst, const void* src, size_t sz);
extern void* memset(void* dst, int v, size_t sz);
extern size_t strlen(const char* s);

#endif