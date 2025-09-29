#include <memmap.h>

static bool __memmap_lt(void* a, void* b) {
	return ((memmap_reg_t*)a)->base < ((memmap_reg_t*)b)->base;
}

void memmap_sort(memmap_reg_t* map, size_t len) {
	memmap_reg_t tmp;
	insertion_sort(map, len, sizeof(memmap_reg_t), __memmap_lt, &tmp);
}