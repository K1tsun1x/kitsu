#pragma once
#ifndef PCI_H
#define PCI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PCI_CSAM_1				1	// configuration space access mechanism 1 supported
#define PCI_CSAM_2				2	// configuration space access mechanism 2 supported
#define PCI_SCGM_1				16	// Special Cycle generation mechanism 1 supported
#define PCI_SCGM_2				32	// Special Cycle generation mechanism 2 supproted

#pragma pack(push, 1)
typedef struct _pci_info_t {
	uint32_t		pmep;		// protected mode entry point
	uint8_t			hwchars;	// hardware characteristics
	uint8_t			iflvmajor;	// interface level major version (BCD)
	uint8_t			iflvminor;	// interface level minor version (BCD)
} pci_info_t;
#pragma pack(pop)

/**
 * Retrieve PCI support information via BIOS (INT 0x1A, AX=0xB101)
 * @note Requires function __int1a_b101() to be implemented
 * @return
 * - `true` - PCI BIOS support detected. The returned values are valid,
 * but only the bits that are set can be trusted. Unset bits must be
 * considered undefined.
 * @return
 * - `false` - PCI BIOS support not detected. In this case the returned
 * values are not guaranteed to be meaningful.
 */
extern bool pciqrbios(pci_info_t* dst);

#endif