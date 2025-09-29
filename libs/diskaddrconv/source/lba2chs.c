#include <diskaddrconv.h>

void lba2chs(
	uint32_t lba,
	uint8_t hpc, uint8_t spt,
	uint16_t* cylinder, uint8_t* head, uint8_t* sector
) {
	if (cylinder) *cylinder = lba / (hpc * spt);		// LBA / (HPC * SPT)
	if (head) *head = (lba / spt) % hpc;				// (LBA / SPT) % HPC
	if (sector) *sector = (lba % spt) + 1;				// (LBA % SPT) + 1
}