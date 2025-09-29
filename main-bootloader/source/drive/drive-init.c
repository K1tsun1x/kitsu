#include <drive.h>

// must return 0
extern uint8_t __13_08(uint8_t disk, uint16_t* dx, uint16_t* cx);

uint16_t __drive_cylinders = 0;
uint16_t __drive_heads = 0;
uint8_t __drive_spt = 0;

bool drive_init(uint8_t disk) {
	uint16_t cx = 0;
	uint16_t dx = 0;
	uint8_t res = __13_08(disk, &dx, &cx);
	if (res) return false;

	__drive_cylinders = (((cx & 0xc0) << 2) | (cx >> 8)) + 1;
	__drive_heads = (dx >> 8) + 1;
	__drive_spt = cx & 0x3f;

	return true;
}