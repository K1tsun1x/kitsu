#include <stdlib.h>

extern const char __STDLIB_DIGITS[];

long long strtoll(const char* str, char** endptr, int radix) {
	const char* s = str;
	while (isspace(*s)) ++s;

	char* tmpendptr;
	bool neg = false;
	if (*s == '-') {
		neg = true;
		++s;
	}
	else if (*s == '+') ++s;

	unsigned long long ures = strtoull(s, &tmpendptr, radix);

	long long res;
	if (neg) {
		if (ures > (unsigned long long)LLONG_MAX + 1) {
			res = LLONG_MIN;
			errno = ERANGE;
		}
		else res = -(long long)ures;
	}
	else {
		if (ures > LLONG_MAX) {
			res = LLONG_MAX;
			errno = ERANGE;
		}
		else res = (long long)ures;
	}

	if (endptr) *endptr = (tmpendptr == s ? (char*)str : tmpendptr);
	return res;
}