# FOSOS
A simple command-line operating system base for developers. It provides a simple but strong base to build your OS on top of, with a simple and permissive [BSD 2-Clause license](https://github.com/Kaimakan71/FOSOS/blob/main/LICENSE).

Sections (in order):
* Authors
* Prerequisites
* Emulator Requirements
* Trying It Out
* Building

## Authors
* Creator & main development - [**Kaimakan71**](https://github.com/Kaimakan71)
* C bug help :~) - [**CoherenceLabs**](https://github.com/CoherenceLabs)
* Helpful articles/tutorials - [**OSDev Wiki**](https://wiki.osdev.org)

## Prerequisites
* [NASM](https://www.nasm.us) (assembler, only needed if modifying bootloader)
* [QEMU](https://www.qemu.org) (emulator for running FOSOS)
* GCC (compiler for kernel code)

## Emulator Requirements
* 1MB RAM or more
* i386 or better processor

## Trying It Out
It's very simple to try FOSOS, as prebuilt binaries are included in the `build` directory. Simply run an emulator with the disk image!
```
qemu-system-i386 -cpu max -m 1M -drive file=build/disk.bin,format=raw,if=floppy
```

## Building
### Normal
To build FOSOS, on Windows run `run` and on Mac/Linux run `./run.sh`
### Bootloader
We provide a precompiled bootloader, and the build files do not rebuild it as it is not commonly modified. However, if you have made changes to it you need to recompile for those changes to take effect. In your run.sh or run.bat file, uncomment the line that says 'compile the bootloader' above it. Then build and you should be good to go.

Copyright (c) 2022, the FOSOS developers.