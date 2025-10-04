#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <mbr.h>

extern void main_help(void);
extern int main_list(const char* path, size_t i);
extern int main_write(
	const char* path,
	size_t i,
	uint8_t flags,
	uint16_t start_cyl, uint8_t start_head, uint8_t start_sector,
	uint8_t sys_id,
	uint16_t end_cyl, uint8_t end_head, uint8_t end_sector,
	uint32_t offset,
	uint32_t length
);

#endif