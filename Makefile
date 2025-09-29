CC=$(TARGET)-gcc
LD=$(TARGET)-gcc
AR=$(TARGET)-ar
AS=nasm
EMU=qemu-system-i386
BUILD_DIR=./build
LIBS_DIR=./libs
RESULT=$(BUILD_DIR)/kitsu.bin
LIB_NUM2STR_RESULT=$(BUILD_DIR)/libnum2str.a
LIB_PRINT_RESULT=$(BUILD_DIR)/libprint.a
LIB_DISKADDRCONV_RESULT=$(BUILD_DIR)/libdiskaddrconv.a
LIB_STRING_RESULT=$(BUILD_DIR)/libstring.a

LIB_INCLUDE_DIRS=$(addsuffix /include, $(addprefix -I, $(wildcard ./libs/*)))

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

LIBS_16_TO_BUILD=diskaddrconv pci stdio e820 memmap sort stdlib string ctype
LIBS_16_BUILT=$(addprefix lib, $(addsuffix .16.a, $(LIBS_16_TO_BUILD)))

# main bootloader
CC_FLAGS_MAIN_BOOTLOADER=$(CC_FLAGS_16) -I ./main-bootloader/include $(addprefix -I $(LIBS_DIR)/, $(addsuffix /include, sort string stdlib diskaddrconv print inlasm))
LD_FLAGS_MAIN_BOOTLOADER=$(LD_FLAGS_16) -T ./main-bootloader/link.ld -L$(BUILD_DIR)/ $(addprefix -l, $(shell find $(BUILD_DIR) -name *.16.a))
C_SOURCES_MAIN_BOOTLOADER=$(shell find ./main-bootloader/source -name *.c)
ASM_SOURCES_MAIN_BOOTLOADER=$(shell find ./main-bootloader/source -name *.asm)
C_OBJECTS_MAIN_BOOTLOADER=$(addsuffix .main-bootloader.o, $(C_SOURCES_MAIN_BOOTLOADER))
ASM_OBJECTS_MAIN_BOOTLOADER=$(addsuffix .main-bootloader.o, $(ASM_SOURCES_MAIN_BOOTLOADER))

all:
	$(MAKE) build-all
	$(MAKE) run
	$(MAKE) clean-all

build-all:
	mkdir -p $(BUILD_DIR)
	$(MAKE) $(LIBS_16_BUILT)

	$(MAKE) build-main-bootloader
	$(MAKE) build-pre-bootloader
	cat $(BUILD_DIR)/pre-bootloader.bin $(BUILD_DIR)/main-bootloader.bin > $(BUILD_DIR)/bootloader.bin
	cp $(BUILD_DIR)/bootloader.bin $(RESULT)

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

%.c.main-bootloader.o:		%.c
	$(CC) -c $< $(CC_FLAGS_16) -I ./main-bootloader/include $(LIB_INCLUDE_DIRS) -o $@

%.asm.main-bootloader.o:	%.asm
	$(AS) -felf $< -o $@

build-pre-bootloader:
	$(AS) -fbin ./pre-bootloader/source/main.asm -o $(BUILD_DIR)/pre-bootloader.bin

run:
	$(EMU) $(EMU_FLAGS)

clean-libs:
	rm -f $(BUILD_DIR)/*.a
	find $(LIBS_DIR)/ -name *.o -type f -delete

clean-bootloader:
	rm -f $(ASM_OBJECTS_MAIN_BOOTLOADER) \
		$(C_OBJECTS_MAIN_BOOTLOADER) \
		$(BUILD_DIR)/bootloader.bin \
		$(BUILD_DIR)/main-bootloader.bin \
		$(BUILD_DIR)/pre-bootloader.bin

clean-all:
	rm -f $(BUILD_DIR)/debug.txt
	$(MAKE) clean-libs
	$(MAKE) clean-bootloader
