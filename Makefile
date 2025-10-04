CC=$(TARGET)-gcc
LD=$(TARGET)-gcc
AR=$(TARGET)-ar
AS=nasm
EMU=qemu-system-i386
BUILD_DIR=./build
LIBS_DIR=./libs
TOOLS_DIR=./tools
STORAGE_DIR=./storage
RESULT=$(BUILD_DIR)/kitsu.bin
LIB_NUM2STR_RESULT=$(BUILD_DIR)/libnum2str.a
LIB_PRINT_RESULT=$(BUILD_DIR)/libprint.a
LIB_DISKADDRCONV_RESULT=$(BUILD_DIR)/libdiskaddrconv.a
LIB_STRING_RESULT=$(BUILD_DIR)/libstring.a

LIB_INCLUDE_DIRS=$(addsuffix /include, $(addprefix -I, $(wildcard ./libs/*)))
LIBS_FOR_TOOLS_INCLUDE_DIRS=$(addprefix -I $(LIBS_DIR)/, $(addsuffix /include/, diskaddrconv fat mbr memmap sort))

# libs 16
LIBS_16_TO_BUILD=mbr diskaddrconv pci stdio e820 memmap sort stdlib string ctype
LIBS_16_BUILT=$(addprefix lib, $(addsuffix .16.a, $(LIBS_16_TO_BUILD)))

# libs 32
LIBS_32_TO_BUILD=$(LIBS_16_TO_BUILD)
LIBS_32_BUILT=$(addprefix lib, $(addsuffix .32.a, $(LIBS_32_TO_BUILD)))

# libs for tools
LIBS_TOOL_TO_BUILD=mbr diskaddrconv memmap sort
LIBS_TOOL_BUILT=$(addprefix lib, $(addsuffix .tool.a, $(LIBS_TOOL_TO_BUILD)))

# emulator
EMU_FLAGS=-monitor stdio -m 2G -cpu max -drive format=raw,file=$(RESULT) -D $(BUILD_DIR)/debug.txt -d int

# flags
CC_FLAGS=-Wall -Wextra -ffreestanding -O0 -std=c11
LD_FLAGS=-Wall -Wextra -ffreestanding -O0 -nostdlib -lgcc

# flags for 16-bit
CC_FLAGS_16=$(CC_FLAGS) -m16
LD_FLAGS_16=$(LD_FLAGS) -m16

# flags for 32-bit
CC_FLAGS_32=$(CC_FLAGS) -m32
LD_FLAGS_32=$(LD_FLAGS) -m32

# flags for tools
CC_FLAGS_TOOL=-Wall -Wextra -O2 -std=c11
LD_FLAGS_TOOL=-Wall -Wextra -O2 -lgcc

# main bootloader
CC_FLAGS_MAIN_BOOTLOADER=$(CC_FLAGS_16) -I ./main-bootloader/include $(LIB_INCLUDE_DIRS)
LD_FLAGS_MAIN_BOOTLOADER=$(LD_FLAGS_16) -T ./main-bootloader/link.ld -L$(BUILD_DIR)/ $(addprefix -l, $(shell find $(BUILD_DIR) -name *.16.a))
C_SOURCES_MAIN_BOOTLOADER=$(shell find ./main-bootloader/source -name *.c)
ASM_SOURCES_MAIN_BOOTLOADER=$(shell find ./main-bootloader/source -name *.asm)
C_OBJECTS_MAIN_BOOTLOADER=$(addsuffix .main-bootloader.o, $(C_SOURCES_MAIN_BOOTLOADER))
ASM_OBJECTS_MAIN_BOOTLOADER=$(addsuffix .main-bootloader.o, $(ASM_SOURCES_MAIN_BOOTLOADER))

# mbrman
C_SOURCES_MBRMAN=$(shell find ./tools/mbrman/source -name *.c)
C_OBJECTS_MBRMAN=$(addsuffix .tool.o, $(C_SOURCES_MBRMAN))

all:
	$(MAKE) build-all
	$(MAKE) run
	$(MAKE) clean-all

build-all:
	mkdir -p $(BUILD_DIR)
	@echo "1) libs 16:\n"
	$(MAKE) $(LIBS_16_BUILT)
	@echo "2) libs 32:\n"
	$(MAKE) $(LIBS_32_BUILT)
	@echo "3) libs for tools:\n"
	$(MAKE) $(LIBS_TOOL_BUILT)

	@echo "4) bootloader:\n"
# bootloader
	$(MAKE) build-main-bootloader
	$(MAKE) build-pre-bootloader
	cat $(BUILD_DIR)/pre-bootloader.bin $(BUILD_DIR)/main-bootloader.bin > $(BUILD_DIR)/bootloader.bin

	@echo "5) tools:\n"
# tools
	$(MAKE) build-tool-mbrman

	@echo "6) kernel:\n"
# kernel
	$(MAKE) build-kernel

# filesystem (FAT32)
	@echo "7) filesystem (FAT32):\n"
	mkdir -p $(STORAGE_DIR)
	mkdir -p $(STORAGE_DIR)/kernel
	cp $(BUILD_DIR)/kernel.elf $(STORAGE_DIR)/kernel
	$(MAKE) build-fs

# result
	@echo "8) result:\n"
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/fat32.bin > $(RESULT)

# mbr
	@echo "9) mbr:\n"
	$(MAKE) build-mbr

build-mbr:
	fssizeinsecs=$$((((($$(wc -c < $(BUILD_DIR)/fat32.bin) + 511) & ~511) / 512))) ; \
	mainbtldrleninsecs=$$((((($$(wc -c < $(BUILD_DIR)/main-bootloader.bin) + 511) & ~511) / 512))) ; \
	fsoffset=$$(((1 + mainbtldrleninsecs))) ; \
	$(BUILD_DIR)/mbrman w $(RESULT) 0 128 0 0 2 127 1023 255 63 1 $$mainbtldrleninsecs ; \
	$(BUILD_DIR)/mbrman w $(RESULT) 1 128 1023 255 63 11 1023 255 63 $$fsoffset $$fssizeinsecs

build-fs:
	cd $(BUILD_DIR) ; \
	dd if=/dev/zero of=./fat32.bin bs=1M count=64 ; \
	mkfs.fat -F 32 ./fat32.bin -n "KITSU OS" ; \
	mdir -i ./fat32.bin :: ; \
	mcopy -i ./fat32.bin -s ../$(STORAGE_DIR)/* :: ; \
	cd ../

build-tool-%:
	@mkdir -p $(BUILD_DIR)
	csrcs="$$(find $(TOOLS_DIR)/$*/source -name '*.c')" ; \
	asmsrcs="$$(find $(TOOLS_DIR)/$*/source -name '*.asm')" ; \
	cobjs=$$(for f in $$csrcs; do \
		cobj="$$f.$*.o"; \
		gcc -c $$f $(LIBS_FOR_TOOLS_INCLUDE_DIRS) -I $(TOOLS_DIR)/$*/include $(CC_FLAGS_TOOL) -o $$cobj; \
		echo $$cobj; \
	done); \
	asmobjs=$$(for f in $$asmsrcs; do \
		asmobj="$$f.$*.o"; \
		nasm -felf $$f -o $$asmobj; \
		echo $$asmobj; \
	done) ; \
	gcc $$cobjs $$asmobjs $(LD_FLAGS_TOOL) \
		-L$(BUILD_DIR)/ $(addprefix -l, $(addsuffix .tool, $(LIBS_TOOL_TO_BUILD))) \
		-o $(BUILD_DIR)/$*

lib%.tool.a:
	@mkdir -p $(BUILD_DIR)
	csrcs="$$(find $(LIBS_DIR)/$*/source -name '*.c')" ; \
	asmsrcs="$$(find $(LIBS_DIR)/$*/source -name '*.asm')" ; \
	cobjs=$$(for f in $$csrcs; do \
		cobj="$$f.tool.o"; \
		gcc -c $$f $(LIB_INCLUDE_DIRS) $(CC_FLAGS_TOOL) -o $$cobj; \
		echo $$cobj; \
	done); \
	asmobjs=$$(for f in $$asmsrcs; do \
		asmobj="$$f.tool.o"; \
		nasm -felf $$f -o $$asmobj; \
		echo $$asmobj; \
	done) ; \
	ar rcs $(BUILD_DIR)/$@ $$cobjs $$asmobjs

build-kernel:
	@mkdir -p $(BUILD_DIR)
	csrcs="$$(find ./kernel/source -name '*.c')" ; \
	asmsrcs="$$(find ./kernel/source -name '*.asm')" ; \
	cobjs=$$(for f in $$csrcs; do \
		cobj="$$f.kernel.o"; \
		$(CC) -c $$f -I ./kenrel/include $(LIB_INCLUDE_DIRS) $(CC_FLAGS_32) -o $$cobj; \
		echo $$cobj; \
	done); \
	asmobjs=$$(for f in $$asmsrcs; do \
		asmobj="$$f.kernel.o"; \
		$(AS) -felf $$f -o $$asmobj; \
		echo $$asmobj; \
	done) ; \
	$(LD) $$cobjs $$asmobjs $(LD_FLAGS_32) -o $(BUILD_DIR)/kernel.elf

lib%.32.a:
	@mkdir -p $(BUILD_DIR)
	csrcs="$$(find $(LIBS_DIR)/$*/source -name '*.c')" ; \
	asmsrcs="$$(find $(LIBS_DIR)/$*/source -name '*.asm')" ; \
	cobjs=$$(for f in $$csrcs; do \
		cobj="$$f.32.o"; \
		$(CC) -c $$f $(LIB_INCLUDE_DIRS) $(CC_FLAGS_32) -o $$cobj; \
		echo $$cobj; \
	done); \
	asmobjs=$$(for f in $$asmsrcs; do \
		asmobj="$$f.32.o"; \
		$(AS) -felf $$f -o $$asmobj; \
		echo $$asmobj; \
	done) ; \
	$(AR) rcs $(BUILD_DIR)/$@ $$cobjs $$asmobjs

lib%.16.a:
	@mkdir -p $(BUILD_DIR)
	csrcs="$$(find $(LIBS_DIR)/$*/source -name '*.c')" ; \
	asmsrcs="$$(find $(LIBS_DIR)/$*/source -name '*.asm')" ; \
	cobjs=$$(for f in $$csrcs; do \
		cobj="$$f.16.o"; \
		$(CC) -c $$f $(LIB_INCLUDE_DIRS) $(CC_FLAGS_16) -o $$cobj; \
		echo $$cobj; \
	done); \
	asmobjs=$$(for f in $$asmsrcs; do \
		asmobj="$$f.16.o"; \
		$(AS) -felf $$f -o $$asmobj; \
		echo $$asmobj; \
	done) ; \
	$(AR) rcs $(BUILD_DIR)/$@ $$cobjs $$asmobjs

build-main-bootloader:
ifneq ($(C_SOURCES_MAIN_BOOTLOADER),)
	$(MAKE) $(C_OBJECTS_MAIN_BOOTLOADER)
endif
ifneq ($(ASM_SOURCES_MAIN_BOOTLOADER),)
	$(MAKE) $(ASM_OBJECTS_MAIN_BOOTLOADER)
endif
	$(LD) \
		$(C_OBJECTS_MAIN_BOOTLOADER) $(ASM_OBJECTS_MAIN_BOOTLOADER) \
		$(LD_FLAGS_16) -T ./main-bootloader/link.ld \
		-L$(BUILD_DIR)/ $(addprefix -l, $(addsuffix .16, $(LIBS_16_TO_BUILD))) \
		-o $(BUILD_DIR)/main-bootloader.bin
	
# append bytes to the end of file (align file size to 512-byte boundary)
	size=$$(wc -c < $(BUILD_DIR)/main-bootloader.bin) ; \
	alignedsize=$$(((($$size + 511) & ~511))) ; \
	truncate -s $$alignedsize $(BUILD_DIR)/main-bootloader.bin

%.c.main-bootloader.o:		%.c
	$(CC) -c $< $(CC_FLAGS_16) -I ./main-bootloader/include $(LIB_INCLUDE_DIRS) -o $@

%.asm.main-bootloader.o:	%.asm
	$(AS) -felf $< -o $@

build-pre-bootloader:
	$(AS) -fbin ./pre-bootloader/source/main.asm -o $(BUILD_DIR)/pre-bootloader.bin

run:
	$(EMU) $(EMU_FLAGS)

clean-all:
	rm -f $(BUILD_DIR)/debug.txt
	rm -f $(BUILD_DIR)/fat32.bin
	rm -f kernel/source/*.kernel.o
	$(MAKE) clean-tools
	$(MAKE) clean-libs
	$(MAKE) clean-bootloader

clean-tools:
	objs="$$(find $(TOOLS_DIR)/ -name '*.o')" ; \
	rm -f $$objs

clean-libs:
	rm -f $(BUILD_DIR)/*.a
	find $(LIBS_DIR)/ -name *.o -type f -delete

clean-bootloader:
	rm -f $(ASM_OBJECTS_MAIN_BOOTLOADER) \
		$(C_OBJECTS_MAIN_BOOTLOADER) \
		$(BUILD_DIR)/bootloader.bin \
		$(BUILD_DIR)/main-bootloader.bin \
		$(BUILD_DIR)/pre-bootloader.bin
