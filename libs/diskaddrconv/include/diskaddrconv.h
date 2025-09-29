#pragma once
#ifndef DISKADDRCONV_H
#define DISKADDRCONV_H

#include <stdint.h>

extern void lba2chs(
	uint32_t lba,
	uint8_t hpc, uint8_t spt,
	uint16_t* cylinder, uint8_t* head, uint8_t* sector
);

#endif