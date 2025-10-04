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
extern int putchar(int c);

/**
 * @warning Requires function putchar() to be implemented
 */
extern int vprintf(const char* format, va_list arg);

/**
 * @warning Requires function putchar() to be implemented
 */
extern int printf(const char* format, ...);


/**
 * @warning Requires function putchar() to be implemented
 */
extern int puts(const char* s);

#endif