OBJS = \
	_start.o \
	init.o \
	stdlib.o \
	drivers/ports.o \
	drivers/vga.o \
	drivers/pic.o \
	drivers/i386.o \
	drivers/keyboard.o \
	drivers/rtc.o \
	drivers/pit.o \
	shell.o

all: build/disk.img

build/disk.img: build/boot.bin build/kernel.bin
	@echo "Creating disk image"
	@cat build/boot.bin build/kernel.bin > build/disk.img

build/kernel.bin: $(OBJS)
	@echo "Linking object files"
	@ld -Tbuild/linker.ld --strip-debug -melf_i386 --gc-sections --build-id=none -z norelro -z now -Ttext 0x10000 -o build/kernel.bin $(OBJS)

build/boot.bin: boot.asm
	@echo "Assembling bootloader"
	@nasm boot.asm -f bin -o build/boot.bin

.c.o:
	@echo "Compiling $<"
	@gcc -Iinclude -Wextra -Wall -Wundef -Wcast-qual -Wwrite-strings -Wno-unused-parameter -Os -fno-asynchronous-unwind-tables -ffreestanding -fno-stack-protector -fno-ident -fomit-frame-pointer -mregparm=3 -march=i386 -m32 -fno-exceptions -ffunction-sections -fdata-sections -fmerge-all-constants -fno-unroll-loops -fno-align-functions -fno-align-jumps -fno-align-loops -nostdinc -nostdlib -Wl,-z,noseparate-code,--build-id=none -o $@ -c $<

clean:
	@echo "Cleaning"
	@rm -f $(OBJS) build/kernel.bin build/boot.bin

run:
	@qemu-system-i386 -drive format=raw,file=build/disk.img,if=floppy -rtc base=localtime -name FOSOS