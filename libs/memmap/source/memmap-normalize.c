#include <memmap.h>

static inline size_t __memmap_strict_type(size_t type) {
	return type > MEMMAP_TYPE_UNACCEPTED ? MEMMAP_TYPE_RESERVED : type;
}

static inline memmap_vtype_t __memmap_max(memmap_vtype_t a, memmap_vtype_t b) {
	/*
	uint32_t la = (uint32_t)(a & 0xffffffff);
	uint32_t ha = (uint32_t)(a >> 32);
	uint32_t lb = (uint32_t)(b & 0xffffffff);
	uint32_t hb = (uint32_t)(b >> 32);
	if (ha > hb) return a;
	else if (ha < hb) return b;
	return la > lb ? a : b;
	*/
	return a > b ? a : b;
}

void memmap_normalize(memmap_reg_t* map, size_t len, size_t* lenout) {
	for (size_t i = 0; i < len; i++) map[i].type = __memmap_strict_type(map[i].type);

	size_t out = 0;
	memmap_reg_t* prev, * cur;
	memmap_vtype_t prevbeg, prevend, curbeg, curend;
	for (size_t i = 1; i < len; i++) {
		prev = &map[out];
		cur = &map[i];

		prevbeg = prev->base;
		prevend = prev->base + prev->len;
		curbeg = cur->base;
		curend = cur->base + cur->len;

		if (curbeg >= prevend) {
			out += 1;
			map[out] = *cur;
			continue;
		}

		if (cur->type == prev->type) prev->len = __memmap_max(prevend, curend) - prevbeg;
		else {
			if (curbeg > prevbeg) {
				prev->len = curbeg - prevbeg;
				out += 1;
				map[out] = *cur;
				map[out].base = prevend;
				map[out].len = curend - prevend;
			}
			else {
				prev->type = __memmap_max((memmap_vtype_t)prev->type, (memmap_vtype_t)cur->type);
				prev->len = __memmap_max(prevend, curend) - prevbeg;
			}
		}
	}

	if (lenout) *lenout = out;
}