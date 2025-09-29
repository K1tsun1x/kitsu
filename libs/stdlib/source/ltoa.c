#include <stdlib.h>

char* ltoa(long n, char* buffer, int radix) {
	if (n < 0) {
		buffer[0] = '-';
		n = -n;
		++buffer;
	}

	return ultoa((unsigned long)n, buffer, radix);
}