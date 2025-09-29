#include <memmap.h>

void memmap_remove_empty(memmap_reg_t* map, size_t len, size_t* lenout) {
	size_t szout = 0;
	for (size_t i = 0; i < len; i++) if (map[i].len) {
		map[szout] = map[i];
		szout += 1;
	}
	
	if (lenout) *lenout = szout;
}