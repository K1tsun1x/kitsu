ENTRY(kitsu_loader_main)
OUTPUT_ARCH(i386)
OUTPUT_FORMAT(binary)
SECTIONS {
	__BEGIN__ = 0x1000;
	. = __BEGIN__;

	.HEADER : {
		SHORT(0x4d10);
		BYTE(((__END__ - __BEGIN__ + 511) & ~(511)) / 512);
		SHORT(kitsu_loader_main);
	}

	. = ALIGN(8);

	.text : {
		*(.text)
	}

	. = ALIGN(8);

	.data : {
		*(.data)
	}

	. = ALIGN(8);

	.bss : {
		*(.bss)
	}

	. = ALIGN(8);

	.rodata : {
		*(.rodata)
		. = . + 0;
	}

	. = ALIGN(512);

	__END__ = .;
}