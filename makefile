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
CCFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -nostdlib -Wall -Wextra -Isrc
NASMFLAGS = -f bin
LDFLAGS = -m elf_i386 -T linker.ld

# dirs and targets
SRC = src
BUILD = build
ELF = ${BUILD}/kaos.k.elf
IMG = ${BUILD}/kaos.img

# file snatch commands
CSRC := $(shell find $(SRC) -type f -name "*.c")
SSRC := $(shell find $(SRC) -type f -name "*.s")
HSRC := $(shell find $(SRC) -type f -name "*.h")

# source files to object files
COBJS := $(CSRC:$(SRC)/%.c=$(BUILD)/%.o)

SOBJS := $(patsubst $(SRC)/%.s,$(BUILD)/%.bin,$(filter %.s,$(SSRC)))

# commands
.PHONY: all run clean

all: $(IMG)

# kernel with linker.ld
$(ELF): $(COBJS) linker.ld
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $(COBJS)

# final img
$(IMG): $(ELF) $(SOBJS)
	@mkdir -p $(dir $@)
	cat $(BUILD)/boot/*.bin > $@ 2>/dev/null || true

# build c files
$(BUILD)/%.o: $(SRC)/%.c $(HSRC)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

# build s files
$(BUILD)/%.bin: $(SRC)/%.s
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

# run command (with qemu)
run: $(IMG)
	qemu-system-i386 -drive format=raw,file=$(IMG)

# clean
clean:
	rm -rf $(BUILD)