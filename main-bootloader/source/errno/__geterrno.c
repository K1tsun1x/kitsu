#include <errno.h>

static int __errno = 0;

int* __geterrno(void) {
	return &__errno;
}