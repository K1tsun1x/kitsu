#include <stdlib.h>

extern const char __STDLIB_DIGITS[];

long strtol(const char* str, char** endptr, int radix) {
	const char* s = str;
	while (isspace(*s)) ++s;

	char* tmpendptr;
	bool neg = false;
	if (*s == '-') {
		neg = true;
		++s;
	}
	else if (*s == '+') ++s;

	unsigned long ures = strtoul(s, &tmpendptr, radix);

	long res;
	if (neg) {
		if (ures > (unsigned long)LONG_MAX + 1) {
			res = LONG_MIN;
			errno = ERANGE;
		}
		else res = -(long)ures;
	}
	else {
		if (ures > LONG_MAX) {
			res = LONG_MAX;
			errno = ERANGE;
		}
		else res = (long)ures;
	}

	if (endptr) *endptr = (tmpendptr == s ? (char*)str : tmpendptr);
	return res;
}