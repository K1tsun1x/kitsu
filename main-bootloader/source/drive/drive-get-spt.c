#include <drive.h>

extern uint8_t __drive_spt;

uint8_t drive_get_spt(void) {
	return __drive_spt;
}