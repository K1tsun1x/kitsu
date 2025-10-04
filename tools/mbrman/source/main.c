#include <main.h>

int main(int argc, char** argv) {
	if (argc < 2) printf("Use `mbrman help` to display usage information\r\n");
	else if (argc >= 2 && !strcmp(argv[1], "help")) main_help();
	else if (argc < 4 && !strcmp(argv[1], "list")) return main_list((const char*)argv[2], (size_t)-1);
	else if (argc >= 4 && !strcmp(argv[1], "list")) return main_list((const char*)argv[2], (size_t)strtoul(argv[3], 0, 10));
	else if (argc >= 14 && !strcmp(argv[1], "w")) return main_write(
		(const char*)argv[2],				// file.bin
		(size_t)strtoul(argv[3], 0, 10),	// index
		(size_t)strtoul(argv[4], 0, 10),	// flags
		(size_t)strtoul(argv[5], 0, 10),	// start cylinder
		(size_t)strtoul(argv[6], 0, 10),	// start head
		(size_t)strtoul(argv[7], 0, 10),	// start sector
		(size_t)strtoul(argv[8], 0, 10),	// system id
		(size_t)strtoul(argv[9], 0, 10),	// end cylinder
		(size_t)strtoul(argv[10], 0, 10),	// end head
		(size_t)strtoul(argv[11], 0, 10),	// end sector
		(size_t)strtoul(argv[12], 0, 10),	// offset
		(size_t)strtoul(argv[13], 0, 10)	// len
	);
	else {
		puts("Error: missing/unknown arguments!\r\nUse `mbrman help` to display usage information");
		return -1;
	}

	return 0;
}