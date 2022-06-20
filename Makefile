OBJS = \
	_start.o \
	init.o \
	stdlib.o \
	io.o \
	drivers/pic.o \
	drivers/vga.o \
	drivers/i386.o \
	drivers/keyboard.o

all: build/disk.img

build/disk.img: build/boot.bin build/kernel.bin
	@echo "Creating disk image"; cat build/boot.bin build/kernel.bin > build/disk.img

build/kernel.bin: $(OBJS)
	@echo "Linking object files"; ld -Tbuild/linker.ld --strip-debug -melf_i386 --gc-sections --build-id=none -z norelro -z now -Ttext 0x10000 -o build/kernel.bin $(OBJS)

build/boot.bin: boot.asm
	@echo "Assembling bootloader"; nasm boot.asm -f bin -o build/boot.bin

.cpp.o:
	@echo "Compiling $<"; g++ -Wextra -Wall -Wundef -Wcast-qual -Wwrite-strings -Os -fno-asynchronous-unwind-tables -ffreestanding -fno-stack-protector -fno-ident -fomit-frame-pointer -mregparm=3 -march=i386 -m32 -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -fmerge-all-constants -fno-unroll-loops -fno-align-functions -fno-align-jumps -fno-align-loops -std=c++17 -nostdinc++ -nostdlib -Iinclude -o $@ -c $<

clean:
	@echo "Cleaning"; rm -f $(OBJS) build/kernel.bin build/boot.bin

run:
	@echo "Running emulator"; qemu-system-i386 -m 4M -drive format=raw,file=build/disk.img,if=floppy