#   =========== KAOS ===========
#
#   This file builds the KAOS operating system.
#   NASM and GCC are required to build the system.
#
#   ============================

# tools
CC = gcc
LD = ld
NASM = nasm

# flags
CCFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -nostdlib -Wall -Wextra -Isrc -I./src/libc/include
LDFLAGS = -m elf_i386 -T linker.ld

# dirs and targets
SRC = src
BUILD = build
ELF = ${BUILD}/kaos.k.elf
IMG = ${BUILD}/kaos.img

# file snatch commands
CSRC := $(shell find $(SRC) -type f -name "*.c")
HSRC := $(shell find $(SRC) -type f -name "*.h")

# separate bootloader and core assembly files
BOOT_SSRC := $(shell find $(SRC)/boot -type f -name "*.s")
CORE_SSRC := $(shell find $(SRC)/core -type f -name "*.s")

# Source files to object/binary files
COBJS := $(CSRC:$(SRC)/%.c=$(BUILD)/%.o)
CORE_OBJS := $(CORE_SSRC:$(SRC)/%.s=$(BUILD)/%.o)
BOOT_OBJS := $(BOOT_SSRC:$(SRC)/boot/%.s=$(BUILD)/boot/%.bin)

# commands
.PHONY: all run clean

all: $(IMG)

# generate binary from kernel elf
${BUILD}/kernel.bin: $(ELF)
	objcopy -O binary $< $@

# kernel with linker.ld (includes C objects and kernel assembly entry)
$(ELF): $(COBJS) $(CORE_OBJS) linker.ld
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $(COBJS) $(CORE_OBJS)

# final img
$(IMG): $(ELF) $(BOOT_OBJS) ${BUILD}/kernel.bin
	@mkdir -p $(dir $@)
	cat $(BUILD)/boot/s1.bin $(BUILD)/boot/s2.bin $(BUILD)/boot/s3.bin $(BUILD)/kernel.bin > $@

# build c files
$(BUILD)/%.o: $(SRC)/%.c $(HSRC)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

# build kernel assembly files (-f elf32 for linking)
$(BUILD)/core/%.o: $(SRC)/core/%.s
	@mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

# build bootloader assembly files (-f bin for raw binaries)
$(BUILD)/boot/%.bin: $(SRC)/boot/%.s
	@mkdir -p $(dir $@)
	$(NASM) -f bin $< -o $@

# run command (with qemu)
run: $(IMG)
	qemu-system-i386 -drive format=raw,file=$(IMG)

# clean
clean:
	rm -rf $(BUILD)