#include <stdlib.h>

size_t ilen(int n, int radix) {
	if (n < 0) n = -n;
	return ulen((unsigned int)n, radix);
}