MAKEFLAGS += -rR
.SUFFIXES:

override OUTPUT := kernel

CC := gcc
LD := ld
ASM := nasm

override CFLAGS += \
    -I src \
    -I src/KrnlAid/stdlib \
    -Wall \
    -Wextra \
    -Werror \
    -std=gnu99 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIE \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mcmodel=kernel \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -g

override LDFLAGS += \
    -nostdlib \
    -static \
	-m elf_x86_64 \
    -z max-page-size=0x1000 \
    -T linker.ld \
    -g

override ASMFLAGS += -Wall -f elf64 

override SRCFILES := $(shell cd src && find -L * -type f | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(CXXFILES:.cpp=.cpp.d) $(ASFILES:.S=.S.d))

.PHONY: all
all: bin/$(OUTPUT)

-include $(HEADER_DEPS)

bin/$(OUTPUT): $(OBJ)
	@mkdir -p "$$(dirname $@)"
	$(LD) $(OBJ) $(LDFLAGS) -o $@

obj/%.c.o: src/%.c
	@mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.asm.o: src/%.asm
	@mkdir -p "$$(dirname $@)"
	$(ASM) $(ASMFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -rf bin obj