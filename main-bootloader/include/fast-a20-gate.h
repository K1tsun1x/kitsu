#pragma once
#ifndef FAST_A20_GATE_H
#define FAST_A20_GATE_H

#include <inlasm.h>
#include <stdbool.h>
#include <stdint.h>

#define FAST_A20_GATE				0x92

#define FAST_A20_GATE_A20			2

// false = bit 1 already set
static inline bool fast_a20_enable_a20(void) {
	uint8_t v = inb(FAST_A20_GATE);
	if (v & FAST_A20_GATE_A20) return false;

	outb(FAST_A20_GATE, v & 0xfe);
	io_wait();
	return true;
}

// false = bit 1 already clear
static inline bool fast_a20_disable_a20(void) {
	uint8_t v = inb(FAST_A20_GATE);
	if (!(v & FAST_A20_GATE_A20)) return false;

	outb(FAST_A20_GATE, v & 0xfc);
	io_wait();
	return true;
}

#endif