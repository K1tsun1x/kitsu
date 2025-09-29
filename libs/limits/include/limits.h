#pragma once
#ifndef LIMITS_H
#define LIMITS_H

#define SCHAR_MIN				-128
#define SCHAR_MAX				127
#define UCHAR_MIN				0
#define UCHAR_MAX				255

#define SHRT_MIN				-32768
#define SHRT_MAX				32767U
#define USHRT_MIN				0
#define USHRT_MAX				65535U

#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN				UCHAR_MIN
#define CHAR_MAX				UCHAR_MAX
#else
#define CHAR_MIN				SCHAR_MIN
#define CHAR_MAX				SCHAR_MAX
#endif

#define INT_MIN					-2147483648
#define INT_MAX					2147483647
#define UINT_MIN				0
#define UINT_MAX				4294967295U

#define LLONG_MAX				9223372036854775807LL
#define LLONG_MIN				(-LLONG_MAX - 1LL)
#define ULLONG_MIN				0
#define ULLONG_MAX				18446744073709551615ULL

#if __WORDSIZE == 64
#define LONG_MIN				LLONG_MIN
#define LONG_MAX				LLONG_MAX
#define ULONG_MIN				ULLONG_MIN
#define ULONG_MAX				ULLONG_MAX
#else
#define LONG_MIN				INT_MIN
#define LONG_MAX				INT_MAX
#define ULONG_MIN				UINT_MIN
#define ULONG_MAX				UINT_MAX
#endif

#endif