#include <stdlib.h>

size_t ulen(unsigned int n, int radix) {
	size_t res = 1;
	while (n /= radix) res += 1;
	return res;
}