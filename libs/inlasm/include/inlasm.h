#pragma once
#ifndef INLASM_H
#define INLASM_H

static inline void outb(uint16_t port, uint8_t value) {
	__asm__ __volatile__("outb %0, %1"::"a"(value), "d"(port));
}

static inline void outw(uint16_t port, uint16_t value) {
	__asm__ __volatile__("outw %0, %1"::"a"(value), "d"(port));
}

static inline void outl(uint16_t port, uint32_t value) {
	__asm__ __volatile__("outl %0, %1"::"a"(value), "d"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t res;
	__asm__ __volatile__("inb %1, %0":"=a"(res):"d"(port));
	return res;
}

static inline uint16_t inw(uint16_t port) {
	uint16_t res;
	__asm__ __volatile__("inw %1, %0":"=a"(res):"d"(port));
	return res;
}

static inline uint32_t inl(uint16_t port) {
	uint32_t res;
	__asm__ __volatile__("inl %1, %0":"=a"(res):"d"(port));
	return res;
}

static inline void sti(void) {
	__asm__ __volatile__("sti");
}

static inline void cli(void) {
	__asm__ __volatile__("cli");
}

static inline void hlt(void) {
	__asm__ __volatile__("jmp .");
}

static inline void io_wait(void) {
	outb(0x80, 0);
}

#endif