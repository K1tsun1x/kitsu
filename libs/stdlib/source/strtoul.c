#include <stdlib.h>

extern const char __STDLIB_DIGITS[];

unsigned long strtoul(const char* str, char** endptr, int radix) {
	const char* s = str;
	while (isspace(*s)) ++s;

	if (*s == '+') ++s;
	else if (*s == '-') ++s;

	if (!radix) {
		if (*s == '0') {
			++s;
			if (tolower(*s) == 'x') {
				radix = 16;
				++s;
			}
			else radix = 8;
		}
		else radix = 10;
	}

	unsigned long res = 0;
	const char* start = s;
	int digit;
	while (*s) {
		if (isdigit(*s)) digit = (int)(*s - '0');
		else if (isalpha(*s)) digit = (int)(tolower(*s) - 'a' + 10);
		else break;

		if (digit >= radix) break;

		if (res > (ULONG_MAX - digit) / radix) {
			res = ULONG_MAX;
			errno = ERANGE;
		}
		else res = res * radix + digit;
		++s;
	}

	if (endptr) *endptr = (char*)(s == start ? str : s);
	return res;
}