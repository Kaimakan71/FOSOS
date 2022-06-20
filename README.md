# FOSOS
A simple command-line operating system base for developers written in C-styled C++. It provides a simple but strong base to build your OS on top of, with a simple and permissive [BSD 2-Clause license](https://github.com/Kaimakan71/FOSOS/blob/main/LICENSE).

## Authors
* Creator & main development - [Kaimakan71](https://github.com/Kaimakan71)
* C bug help :~) - [CoherenceLabs](https://github.com/CoherenceLabs)
* Helpful articles/tutorials - [OSDev Wiki](https://wiki.osdev.org)

## Prerequisites
* [NASM](https://www.nasm.us) (assembler)
* [QEMU](https://www.qemu.org) (emulator)
* Make (build system)
* GCC (compiler collection)

## Running
It's easy to try FOSOS, as a prebuilt binary is included with each commit. You can use the makefile command `make run` to start QEMU.

## Building
Building FOSOS is also very simple. You can just run `make` and an updated disk image will be created. Because FOSOS uses Make, only changed files are recompiled, saving build time.

Copyright (c) 2022, the FOSOS developers.