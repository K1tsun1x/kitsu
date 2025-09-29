#include <drive.h>

extern uint16_t __drive_heads;

uint16_t drive_get_heads(void) {
	return __drive_heads;
}