#include <main.h>

void main_help(void) {
	puts("Usage: `mbrman <command> [arguments...]`\r\nCommands & arguments:");
	puts("`help` - display usage information");
	puts("`list file.bin` - display information about MBR partitions (EBR n/s)");
	puts("`list file.bin index` - display information about MBR partition");
	puts("\tindex - index of partition");
	puts("`w file.bin index flags start_cylinder start_head start_sector sytem_id end_cylinder end_head end_sector offset len` - write MBR partition");
	puts("\tindex - index of partition");
	puts("\tstart_cylinder, start_head, start_sector - start cylinder, head & sector");
	puts("\tsystem_id - system id");
	puts("\tend_cylinder, end_head, end_sector - end cylinder, head & sector");
	puts("\toffset - LBA offset");
	puts("\tlen - length");
}