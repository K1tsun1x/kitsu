#include <e820.h>

extern bool __15_e820(uint32_t ebx, uint16_t di, uint32_t* size, uint32_t* next);

static inline size_t __e820_type_to_memmap_type(uint32_t type) {
	switch(type) {
		case E820_ARD_TYPE_MEM: return MEMMAP_TYPE_NORMAL;
		case E820_ARD_TYPE_RESERVED: return MEMMAP_TYPE_RESERVED;
		case E820_ARD_TYPE_ACPI: return MEMMAP_TYPE_ACPI;
		case E820_ARD_TYPE_NVS: return MEMMAP_TYPE_NVS;
		case E820_ARD_TYPE_UNUSABLE: return MEMMAP_TYPE_UNUSABLE;
		case E820_ARD_TYPE_DISABLED: return MEMMAP_TYPE_DISABLED;
		case E820_ARD_TYPE_PERSISTENT: return MEMMAP_TYPE_PERSISTENT;
		case E820_ARD_TYPE_UNACCEPTED: return MEMMAP_TYPE_UNACCEPTED;
		default: return MEMMAP_TYPE_UNKNOWN;
	}
}

bool e820_get_map(memmap_reg_t* map, size_t maxlen, size_t* lenout) {
	e820_ard_t tmp;
	uint32_t size = 0, next = 0;
	bool ok = __15_e820(0, ((uint32_t)&tmp) & 0xffff, &size, &next);
	if (!ok) return false;

	size_t clength = 0;
	while (clength < maxlen) {
		if (size >= 24) {
			if (!(tmp.ea & 1)){
				tmp.type = E820_ARD_TYPE_RESERVED;
				tmp.ea = 0;
			}
		}
		else tmp.ea = 1;

		map[clength].base = tmp.base_addr;
		map[clength].len = tmp.length;
		map[clength].type = __e820_type_to_memmap_type(tmp.type);

		clength += 1;
		if (!ok || !next) break;

		ok = __15_e820(next, ((uint32_t)(&tmp)) & 0xffff, &size, &next);
	}

	if (lenout) *lenout = clength;
	return true;
}