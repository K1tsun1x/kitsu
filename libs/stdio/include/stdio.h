#pragma once
#ifndef STDIO_H
#define STDIO_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifndef va_arg
#define va_arg(...)
#endif

/**
 * @warning Must be implemented by the target environment
 */
extern void putchar(char c);

/**
 * @warning Requires function putchar() to be implemented
 */
extern void vprintf(const char* format, va_list arg);

/**
 * @warning Requires function putchar() to be implemented
 */
extern void printf(const char* format, ...);


/**
 * @warning Requires function putchar() to be implemented
 */
static inline void puts(const char* s) {
	char* p = (char*)&s[0];
	for (; *p;) putchar(*p++);
}

#endif