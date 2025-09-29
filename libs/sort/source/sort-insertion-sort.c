#include <sort.h>

void insertion_sort(void* arr, size_t len, size_t szel, bool (*ltfn)(void* a, void* b), void* tmpel) {
	ptrdiff_t j;
	for (size_t i = 1; i < len; i++) {
		memcpy(tmpel, (unsigned char*)arr + i * szel, szel);
		j = i - 1;

		while (j >= 0 && ltfn(tmpel, (unsigned char*)arr + j * szel)) {
			memcpy((unsigned char*)arr + (j + 1) * szel, (unsigned char*)arr + j * szel, szel);
			j -= 1;
		}

		memcpy((unsigned char*)arr + (j + 1) * szel, tmpel, szel);
	}
}