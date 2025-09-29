#include <string.h>

void* memcpy(void* dst, const void* src, size_t sz) {
	const unsigned char* csrc = (const unsigned char*)src;
	unsigned char* cdst = (unsigned char*)dst;
	unsigned char* edst = cdst + sz;
	for (; cdst < edst;) *cdst++ = *csrc++;
	return dst;
}