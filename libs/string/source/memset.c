#include <string.h>

void* memset(void* dst, int c, size_t sz) {
	unsigned char cc = (unsigned char)(c & 0xff);
	unsigned char* cdst = (unsigned char*)dst;
	unsigned char* e = cdst + sz;
	for (; cdst < e;) *cdst++ = cc;
	return dst;
}