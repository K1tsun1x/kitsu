#include <stdio.h>
#include <drive.h>
#include <cpu.h>
#include <i8042.h>
#include <fast-a20-gate.h>
#include <e820.h>
#include <memmap.h>

extern char __BEGIN__[];
extern char __END__[];

static memmap_reg_t MEM_MAP[128];
static size_t MEM_MAP_LEN = 0;

static const memmap_reg_t MEM_MAP_EX[] = {
	{
		.base=0x00000000,
		.len=0x00000500,
		.type=MEMMAP_TYPE_RESERVED
	},	 //	IVT & BDA
	/*
	{
		.base=(uint32_t)&__BEGIN__,
		.len=(uint32_t)&__END__ - (uint32_t)&__BEGIN__,
		.type=MEMMAP_TYPE_RESERVED
	},	//	kitsuLoader
	*/
	{
		.base=0x00080000,
		.len=0x00080000,
		.type=MEMMAP_TYPE_RESERVED
	},	//	video, EBDA, motherboard BIOS, ...
	{
		.base=0x00f00000,
		.len=0x00100000,
		.type=MEMMAP_TYPE_RESERVED
	}	//	ISA hole
};

extern void kitsu_loader_main(uint8_t drive) {
	printf(
		"KitsuLoader v%u.%u\r\nLoaded at %#.4x-%#.4x (%u bytes)\r\n",
		0, 1, (unsigned int)&__BEGIN__, (unsigned int)&__END__, (unsigned int)&__END__ - (unsigned int)&__BEGIN__
	);
	printf("Booted from drive %#.2x\r\n", drive);
	printf("Getting drive info...");

	if (drive_init(drive)) printf("[SUCCESS]\r\n");
	else {
		printf("[FAIL]");
		hlt();
	}

	printf("BIOS-reported geometry: ");
	printf("Cylinders=%u, Heads=%u, Sectors/Track=%u\r\n", drive_get_cylinders(), drive_get_heads(), drive_get_spt());

	printf("Checking for presence of i8042 controller...");
	bool i8042isprsnt = i8042_is_present();
	printf(i8042isprsnt ? "[DETECTED]\r\n" : "[NOT DETECTED]\r\n");

	bool a20lnisenbld = cpu_a20_is_enabled();
	printf("Checking A20 line status...");
	if (a20lnisenbld) printf("[ENABLED]\r\n");
	else {
		printf("[DISABLED]\r\n");
		if (i8042isprsnt) {
			printf("Trying to enable A20 line using i8042...");
			i8042_enable_a20();
			a20lnisenbld = cpu_a20_is_enabled();
			if (a20lnisenbld) printf("[SUCCESS]\r\n");
			else printf("[FAIL]\r\n");
		}

		if (!a20lnisenbld) {
			printf("Trying to enable A20 line using fast gate...");
			(void)(fast_a20_enable_a20());
			a20lnisenbld = cpu_a20_is_enabled();
			if (a20lnisenbld) printf("[SUCCESS]\r\n");
			else printf("[FAIL]\r\n");
		}

		if (!a20lnisenbld) hlt();
	}

	printf("Getting memory map (E820)...");
	if (!e820_get_map(MEM_MAP, sizeof(MEM_MAP) / sizeof(MEM_MAP[0]), &MEM_MAP_LEN)) {
		printf("[FAIL]\r\n");
		hlt();
	}

	printf("[SUCCESS]\r\n");

	memmap_sort(MEM_MAP, MEM_MAP_LEN);
	memmap_remove_extra(MEM_MAP, MEM_MAP_LEN, MEM_MAP_EX, sizeof(MEM_MAP_EX) / sizeof(MEM_MAP_EX[0]), &MEM_MAP_LEN);
	memmap_remove_empty(MEM_MAP, MEM_MAP_LEN, &MEM_MAP_LEN);
	memmap_sort(MEM_MAP, MEM_MAP_LEN);

	{
		size_t lobase, hibase, lolen, hilen;
		for (size_t i = 0; i < MEM_MAP_LEN; i++) {
			lobase = MEM_MAP[i].base & 0xffffffff;
			hibase = MEM_MAP[i].base >> 32;
			lolen = MEM_MAP[i].len & 0xffffffff;
			hilen = MEM_MAP[i].len >> 32;
			printf(
				"%u) B: %#.8x%.8x, L: %#.8x%.8x, T: %u\r\n",
				i + 1,
				hibase, lobase,
				hilen, lolen,
				MEM_MAP[i].type
			);
		}
	}

	printf("Done.\r\n");
	hlt();
}