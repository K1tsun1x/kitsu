#include <stdlib.h>

char* itoa(int n, char* buffer, int radix) {
	if (n < 0) {
		buffer[0] = '-';
		n = -n;
		++buffer;
	}

	return utoa((unsigned int)n, buffer, radix);
}