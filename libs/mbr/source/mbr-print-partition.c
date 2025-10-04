#include <mbr.h>

void mbr_print_partition(const mbr_partition_t* partition) {
	if (
		!(partition->start_head == 0 && partition->start_seccyl == 0) &&
		!(partition->end_head == 0 && partition->end_seccyl == 0) &&
		partition->sys_id != 0 &&
		partition->len != 0
	) {
		printf("\r\nFlags: %#x\r\n", partition->flags);
		printf(
			"Start CHS: Cylinder=%#x, Head=%#x, Sector=%#x\r\n",
			((partition->start_seccyl & 0xC0) << 2) | (partition->start_seccyl >> 8),
			partition->start_head,
			partition->start_seccyl & 0x3f
		);
		printf("System ID: %#x\r\n", partition->sys_id);
		printf(
			"End CHS: Cylinder=%#x, Head=%#x, Sector=%#x\r\n",
			((partition->end_seccyl & 0xC0) << 2) | (partition->end_seccyl >> 8),
			partition->end_head,
			partition->end_seccyl & 0x3f
		);
		printf("LBA offset: %#x\r\n", partition->offset);
		printf("Length: %#x", partition->len);
	}
	else printf("empty");
}