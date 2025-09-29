#pragma once
#ifndef DRIVE_H
#define DRIVE_H

#include <diskaddrconv.h>
#include <stdbool.h>

extern bool drive_init(uint8_t disk);
extern uint16_t drive_get_cylinders(void);
extern uint16_t drive_get_heads(void);
extern uint8_t drive_get_spt(void);

#endif