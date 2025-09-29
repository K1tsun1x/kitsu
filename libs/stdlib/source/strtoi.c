#include <stdlib.h>

extern const char __STDLIB_DIGITS[];

int strtoi(const char* str, char** endptr, int radix) {
	const char* s = str;
	while (isspace(*s)) ++s;

	char* tmpendptr;
	bool neg = false;
	if (*s == '-') {
		neg = true;
		++s;
	}
	else if (*s == '+') ++s;

	unsigned int ures = strtoul(s, &tmpendptr, radix);

	int res;
	if (neg) {
		if (ures > (unsigned int)INT_MAX + 1) {
			res = INT_MIN;
			errno = ERANGE;
		}
		else res = -(int)ures;
	}
	else {
		if (ures > INT_MAX) {
			res = INT_MAX;
			errno = ERANGE;
		}
		else res = (int)ures;
	}

	if (endptr) *endptr = (tmpendptr == s ? (char*)str : tmpendptr);
	return res;
}