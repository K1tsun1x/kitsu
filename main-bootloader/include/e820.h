#pragma once
#ifndef E820_H
#define E820_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <memmap.h>

#define E820_SMAP_SIGNATURE				0x534d4150

#define E820_ARD_TYPE_MEM				1		// usable
#define E820_ARD_TYPE_RESERVED			2		// unusable
#define E820_ARD_TYPE_ACPI				3		// can be used after reading ACPI tables
#define E820_ARD_TYPE_NVS				4		// unusable
#define E820_ARD_TYPE_UNUSABLE			5		// unusable
#define E820_ARD_TYPE_DISABLED			6		// unusable
#define E820_ARD_TYPE_PERSISTENT		7		// maybe usable(?)
#define E820_ARD_TYPE_UNACCEPTED		8		// ?

#pragma pack(push, 1)
// address range descriptor
typedef struct _e820_ard_t {
	union {
		struct {
			uint32_t		base_addr_low;
			uint32_t		base_addr_high;
			uint32_t		length_low;
			uint32_t		length_high;
		};
		struct {
			uint64_t		base_addr;
			uint64_t		length;	
		};
	};
	
	uint32_t				type;
	uint32_t				ea;				// extended attributes
} e820_ard_t;
#pragma pack(pop)

/**
 * @warning Requires function __15_e820() to be implemented
 */
extern bool e820_get_map(memmap_reg_t* map, size_t maxlen, size_t* lenout);

#endif