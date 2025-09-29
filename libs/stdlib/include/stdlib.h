#pragma once
#ifndef STDLIB_H
#define STDLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

extern size_t ilen(int n, int radix);
extern size_t ulen(unsigned int n, int radix);
extern size_t llen(long n, int radix);
extern size_t ullen(unsigned long n, int radix);

/**
 * @warning This function can only be used if 64-bit arithmetic
 * operations (add, subtract, multiply, divide) are implemented.
 */
extern size_t lllen(long long n, int radix);

/**
 * @warning This function can only be used if 64-bit arithmetic
 * operations (add, subtract, multiply, divide) are implemented.
 */
extern size_t ulllen(unsigned long long n, int radix);

/* ===================================
 ------------------------------------
=================================== */

extern char* itoa(int n, char* buffer, int radix);
extern char* utoa(unsigned int n, char* buffer, int radix);

extern char* ltoa(long n, char* buffer, int radix);
extern char* ultoa(unsigned long n, char* buffer, int radix);

/**
 * @warning This function can only be used if 64-bit arithmetic
 * operations (add, subtract, multiply, divide) are implemented.
 */
extern char* lltoa(long long n, char* buffer, int radix);

/**
 * @warning This function can only be used if 64-bit arithmetic
 * operations (add, subtract, multiply, divide) are implemented.
 */
extern char* ulltoa(unsigned long long n, char* buffer, int radix);

/* ===================================
 ------------------------------------
=================================== */

#define atoi(__str)			(strtoi(__str, 0, 10))
#define atou(__str)			(strtou(__str, 0, 10))

extern int strtoi(const char* str, char** endptr, int radix);
extern unsigned int strtou(const char* str, char** endptr, int radix);
extern long strtol(const char* str, char** endptr, int radix);
extern unsigned long strtoul(const char* str, char** endptr, int radix);

/**
 * @warning This function can only be used if 64-bit arithmetic
 * operations (add, subtract, multiply, divide) are implemented.
 */
extern long long strtoll(const char* str, char** endptr, int radix);

/**
 * @warning This function can only be used if 64-bit arithmetic
 * operations (add, subtract, multiply, divide) are implemented.
 */
extern unsigned long long strtoull(const char* str, char** endptr, int radix);

#endif