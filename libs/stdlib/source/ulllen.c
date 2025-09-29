#include <stdlib.h>

size_t ulllen(unsigned long long n, int radix) {
	size_t res = 1;
	while (n /= radix) res += 1;
	return res;
}