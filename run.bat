@echo off

@REM compile the bootloader
@REM nasm boot.asm -f bin -o build/boot.bin

@REM compile the kernel
gcc -Wall -Wextra -Werror kernel.c -ffreestanding -nostdlib -m32 -g -c -Wl,--oformat=binary -o build/kernel.bin -I include

cd build

@REM combine the boot and kernel binaries into a disk image
type boot.bin kernel.bin > disk.bin

@REM run an emulator with our disk file
qemu-system-i386 -cpu max -m 1M -drive file=disk.bin,format=raw,if=floppy

cd ..
cls