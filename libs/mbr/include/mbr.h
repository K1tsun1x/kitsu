#pragma once
#ifndef MBR_H
#define MBR_H

#include <diskaddrconv.h>
#include <stddef.h>
#include <stdint.h>

#define MBR_PARTITION_FLAG_ACTIVE			0x80
#define MBR_SIGNATURE						0xaa55

#pragma pack(push, 1)
typedef struct _mbr_partition_t {
	uint8_t			flags;					// MBR_PARTITION_FLAG_...
	uint8_t			start_head;
	uint16_t		start_seccyl;
	uint8_t			sys_id;
	uint8_t			end_head;
	uint16_t		end_seccyl;
	uint32_t		offset;
	uint32_t		len;
} mbr_partition_t;

typedef struct _mbr_t {
	uint8_t			data[446];
	mbr_partition_t	partitions[4];
	uint16_t		signature;				// MBR_SIGNATURE
} mbr_t;
#pragma pack(pop)

/**
 * @warning Requires function printf() to be implemented
 */
extern void mbr_print_partition(const mbr_partition_t* partition);

extern void mbr_init_partition(
	mbr_partition_t* partition,
	uint8_t flags,
	uint16_t start_cyl, uint8_t start_head, uint8_t start_sector,
	uint8_t sys_id,
	uint16_t end_cyl, uint8_t end_head, uint8_t end_sector,
	uint32_t offset,
	uint32_t len
);

/**
 * @warning Requires function printf() to be implemented
 */
static inline void mbr_print_partitions(const mbr_partition_t* partitions, size_t start, size_t len) {
	size_t i = start, end = start + len;
	for (; i < end; i++) mbr_print_partition(&partitions[i]);
}

#endif