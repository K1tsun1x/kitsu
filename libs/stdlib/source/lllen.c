#include <stdlib.h>

size_t lllen(long long n, int radix) {
	if (n < 0) n = -n;
	return ulllen((unsigned long long)n, radix);
}