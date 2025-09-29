#include <stdlib.h>

char* lltoa(long long n, char* buffer, int radix) {
	if (n < 0) {
		buffer[0] = '-';
		n = -n;
		++buffer;
	}

	return ulltoa((unsigned long long)n, buffer, radix);
}