#include <string.h>

size_t strlen(const char* s) {
	char* p = (char*)&s[0];
	for (; *p;) ++p;
	return (size_t)(p - s);
}