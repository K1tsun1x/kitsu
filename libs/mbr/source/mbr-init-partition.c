#include <mbr.h>

void mbr_init_partition(
	mbr_partition_t* partition,
	uint8_t flags,
	uint16_t start_cyl, uint8_t start_head, uint8_t start_sector,
	uint8_t sys_id,
	uint16_t end_cyl, uint8_t end_head, uint8_t end_sector,
	uint32_t offset,
	uint32_t len
) {
	partition->flags = flags;
	partition->start_head = start_head;
	partition->start_seccyl = (start_cyl << 8) | ((start_cyl >> 2) & 0xc0) | (start_sector & 0x3f);
	partition->sys_id = sys_id;
	partition->end_head = end_head;
	partition->end_seccyl = (end_cyl << 8) | ((end_cyl >> 2) & 0xc0) | (end_sector & 0x3f);
	partition->offset = offset;
	partition->len = len;
}