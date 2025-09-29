#include <i8042.h>

void i8042_disable_a20(void) {
	cli();
	i8042_disable_kbd();
	i8042_disable_mouse();
	i8042_flush_buf();

	uint8_t ctrlout = i8042_read_cntrl_out();
	i8042_write_cntrl_out(ctrlout & ~I8042_CONTROLLER_OUTPUT_A20_GATE);
	
	i8042_enable_kbd();
	i8042_enable_mouse();
	sti();
}