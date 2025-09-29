#include <stdlib.h>

size_t ullen(unsigned long n, int radix) {
	size_t res = 1;
	while (n /= radix) res += 1;
	return res;
}