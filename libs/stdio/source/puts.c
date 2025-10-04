#include <stdio.h>

int puts(const char* s) {
	char* p = (char*)&s[0];
	for (; *p;) putchar(*p++);
	putchar('\r');
	putchar('\n');
	return 1;
}