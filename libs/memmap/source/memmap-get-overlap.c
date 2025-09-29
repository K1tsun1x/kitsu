#include <memmap.h>

size_t memmap_get_overlap(const memmap_reg_t* a, const memmap_reg_t* b) {
	size_t abeg = (size_t)a->base, asz = (size_t)a->len;
	size_t bbeg = (size_t)b->base, bsz = (size_t)b->len;
	size_t aend = abeg + asz, bend = bbeg + bsz;

	// ---[AAA]-
	// -[BBB]---
	if (bbeg <= abeg && bend > abeg && bend < aend) return MEMMAP_OVERLAP_BEGIN;

	// -[AAA]---
	// ---[BBB]-
	else if (bbeg > abeg && bbeg < aend && bend >= aend) return MEMMAP_OVERLAP_END;

	// --[AAA]--
	// -[BBBBB]-
	else if (bbeg <= abeg && bend >= aend) return MEMMAP_OVERLAP_OUTSIDE;

	// -[AAAAA]-
	// --[BBB]--
	else if (bbeg >= abeg && bend <= aend) return MEMMAP_OVERLAP_INSIDE;
	return MEMMAP_OVERLAP_NONE;
}