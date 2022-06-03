#!/bin/sh

# compile the bootloader
# nasm boot.asm -f bin -o build/boot.bin

# compile the kernel
gcc -Wall -Wextra -Werror kernel.c -ffreestanding -nostdlib -m32 -g -c -Wl,--oformat=binary -o build/kernel.o -I include
objcopy -O binary -j .text build/kernel.o build/kernel.bin

cd build

# combine the boot and kernel binaries into a disk image
cat boot.bin kernel.bin > disk.bin

# run an emulator with our disk file
qemu-system-i386 -cpu max -m 1M -drive file=disk.bin,format=raw,if=floppy

cd ..
clear