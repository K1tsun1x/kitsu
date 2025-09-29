#pragma once
#ifndef MEMMAP_H
#define MEMMAP_H

#include <stddef.h>
#include <stdint.h>
#include <sort.h>

#define MEMMAP_TYPE_NORMAL					1		// usable
#define MEMMAP_TYPE_RESERVED				2		// unusable
#define MEMMAP_TYPE_ACPI					3		// can be used after reading ACPI tables
#define MEMMAP_TYPE_NVS						4		// unusable
#define MEMMAP_TYPE_UNUSABLE				5		// unusable
#define MEMMAP_TYPE_DISABLED				6		// unusable
#define MEMMAP_TYPE_PERSISTENT				7		// maybe usable(?)
#define MEMMAP_TYPE_UNACCEPTED				8		// ?
#define MEMMAP_TYPE_UNKNOWN					((size_t)-1)

#define MEMMAP_OVERLAP_NONE					0
#define MEMMAP_OVERLAP_BEGIN				1
#define MEMMAP_OVERLAP_END					2
#define MEMMAP_OVERLAP_INSIDE				3
#define MEMMAP_OVERLAP_OUTSIDE				4

typedef uint64_t memmap_vtype_t;
typedef struct _memmap_reg_t {
	memmap_vtype_t		base;
	memmap_vtype_t		len;
	size_t				type;
} memmap_reg_t;

extern void memmap_sort(memmap_reg_t* map, size_t len);
extern void memmap_remove_extra(memmap_reg_t* map, size_t len, const memmap_reg_t* extra, size_t exlen, size_t* outlen);
extern void memmap_remove_empty(memmap_reg_t* map, size_t len, size_t* lenout);
extern void memmap_normalize(memmap_reg_t* map, size_t len, size_t* lenout);
extern size_t memmap_get_overlap(const memmap_reg_t* a, const memmap_reg_t* b);

#endif