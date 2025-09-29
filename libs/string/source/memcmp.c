#include <string.h>

int memcmp(const void* a, const void* b, size_t sz) {
	const unsigned char* ca = (const unsigned char*)a;
	const unsigned char* cb = (const unsigned char*)b;
	const unsigned char* ea = ca + sz;
	for (; ca < ea; ca++, cb++) if (*ca != *cb) return (int)*ca - (int)*cb;
	return 0;
}