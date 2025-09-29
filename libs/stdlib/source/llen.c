#include <stdlib.h>

size_t llen(long n, int radix) {
	if (n < 0) n = -n;
	return ullen((unsigned long)n, radix);
}