#include <drive.h>

extern uint16_t __drive_cylinders;

uint16_t drive_get_cylinders(void) {
	return __drive_cylinders;
}