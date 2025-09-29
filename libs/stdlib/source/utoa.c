#include <stdlib.h>

extern const char __STDLIB_DIGITS[];

char* utoa(unsigned int n, char* buffer, int radix) {
	if (radix < 2 || radix > 36) return 0;
	if (!n) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return buffer;
	}

	char* p = &((char*)buffer)[ulen(n, radix)];
	*p = '\0';

	for (--p; n; --p) {
		*p = __STDLIB_DIGITS[n % radix];
		n /= radix;
	}

	return buffer;
}