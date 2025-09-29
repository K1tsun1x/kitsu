#include <memmap.h>
#include <stdio.h>

void memmap_remove_extra(memmap_reg_t* map, size_t len, const memmap_reg_t* extra, size_t exlen, size_t* lenout) {
	size_t mybeg, mysz, myend, exbeg, exsz, exend;
	size_t j, k, ovl;
	for (size_t i = 0; i < len; i++) {
		if (map[i].type == MEMMAP_TYPE_NORMAL || map[i].type == MEMMAP_TYPE_ACPI) {
			for (j = 0; j < exlen; j++) {
				if (extra[j].len) {
					ovl = memmap_get_overlap(&map[i], &extra[j]);

					mybeg = (size_t)map[i].base;
					mysz = (size_t)map[i].len;
					myend = mybeg + mysz;

					exbeg = (size_t)extra[j].base;
					exsz = (size_t)extra[j].len;
					exend = exbeg + exsz;

					if (ovl == MEMMAP_OVERLAP_BEGIN) {
						map[i].len -= exend - mybeg;
						map[i].base = exend;
					}
					else if (ovl == MEMMAP_OVERLAP_END) map[i].len = exbeg - mybeg;
					else if (ovl == MEMMAP_OVERLAP_INSIDE) {
						for (k = len; k > i; k--) map[k] = map[k - 1];
						map[i + 1].base = exend;
						map[i + 1].len = myend - exend;
						map[i + 1].type = map[i].type;
						map[i].len = exbeg - mybeg;
						len += 1;
					}
					else if (ovl == MEMMAP_OVERLAP_OUTSIDE) map[i].type = MEMMAP_TYPE_RESERVED;
				}
			}
		}
	}

	if (lenout) *lenout = len;
}