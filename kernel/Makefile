MAKEFLAGS += -rR
.SUFFIXES:

override OUTPUT := kernel

CC := clang
LD := ld
ASM := nasm

override CFLAGS += \
    -std=gnu11 \
    -target x86_64-unknown-none \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel \
    -I src \
	-Wall \
    -Werror \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -ffunction-sections \
    -fdata-sections \
    -DLIMINE_API_REVISION=2 \
    -MMD \
    -MP

override LDFLAGS += \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -gc-sections \
    -T linker.ld \
	-m elf_x86_64

override ASMFLAGS += -f elf64 

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