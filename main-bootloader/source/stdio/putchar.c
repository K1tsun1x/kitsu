extern void __int10_0e(char c);

int putchar(int c) {
	__int10_0e((char)c);
	return c;
}