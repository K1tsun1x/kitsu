#include <pci.h>

extern bool __int1a_axb101(uint32_t* edx, uint32_t* edi, uint8_t* al, uint8_t* bh, uint8_t* bl);

bool pciqrbios(pci_info_t* dst) {
	uint32_t signature = 0;
	return (
		__int1a_axb101(&signature, &dst->pmep, &dst->hwchars, &dst->iflvmajor, &dst->iflvminor) &&
		signature == 0x20494350
	);
}