#include <stdio.h>

int vprintf(const char* format, va_list arg) {
	char* p = (char*)&format[0];
	char tmp_buffer[132];
	bool plussign;
	bool anysign;
	bool litpfx;
	size_t prec;
	bool num;
	char* tbuf;
	size_t j;
	size_t nlen;
	size_t rdx;
	for (; *p; p++) {
		plussign = false;
		anysign = true;
		litpfx = false;
		num = false;
		prec = 0;
		rdx = 0;

		if (*p == '%') {
			++p;
			while (*p) {
				if (*p == '#') litpfx = true;
				else if (*p == '+') plussign = true;
				else if (*p == ' ') anysign = false;
				else break;
				++p;
			}

			if (*p == '.') {
				++p;
				prec = strtoul(p, &p, 10);
			}
			
			if (*p == '%') putchar('%');
			else if (*p == 'c') putchar((char)va_arg(arg, long));
			else if (*p == 's') {
				tbuf = va_arg(arg, char*);
				for (j = 0; tbuf[j]; j++) putchar(tbuf[j]);
			}
			else if (*p == 'd') {
				num = true;
				itoa((int)va_arg(arg, long), tmp_buffer, 10);
			}
			else if (*p == 'u') {
				num = true;
				utoa((unsigned int)va_arg(arg, unsigned long), tmp_buffer, 10);
			}
			else if (*p == 'l') {
				num = true;
				if (*++p == 'u') ultoa(va_arg(arg, unsigned long), tmp_buffer, 10);
#if defined(__x86_64__)
				else if (*p == 'l') {
					if (*++p == 'u') ulltoa(va_arg(arg, unsigned long long), tmp_buffer, 10);
					else lltoa(va_arg(arg, long long), tmp_buffer, 10);
				}
#endif
				else ltoa(va_arg(arg, long), tmp_buffer, 10);
			}
			else if (*p == 'x') {
				num = true;
				if (litpfx) rdx = 16;
				ultoa(va_arg(arg, unsigned long), tmp_buffer, 16);
			}
			else if (*p == 'o') {
				num = true;
				if (litpfx) rdx = 8;
				ultoa(va_arg(arg, unsigned long), tmp_buffer, 8);
			}
			else if (*p == 'b') {
				num = true;
				if (litpfx) rdx = 2;
				ultoa(va_arg(arg, unsigned long), tmp_buffer, 2);
			}

			if (num) {
				if (!anysign && *tmp_buffer == '-') tbuf = &tmp_buffer[1];
				else {
					if (plussign && *tmp_buffer != '-') putchar('+');
					tbuf = tmp_buffer;
				}

				if (litpfx) {
					if (rdx == 16) {
						putchar('0');
						putchar('x');
					}
					else if (rdx == 8) putchar('0');
					else if (rdx == 2)  {
						putchar('0');
						putchar('b');
					}
				}
				
				nlen = strlen(tbuf);
				if (nlen < prec) {
					prec -= nlen;
					for (j = 0; j < prec; ++j) putchar('0');
				}

				for (j = 0; tbuf[j]; j++) putchar(tbuf[j]);
			}
		}
		else putchar(*p);
	}

	return 1;
}