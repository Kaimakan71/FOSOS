/*
 * FOSOS IDE driver
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <vga.h>
#include <io.h>
#include <i386.h>
#include <pic.h>

#define IRQ_ATA_PRIMARY 14
#define IDE_STATUS 0x1f7
#define IDE_CMD 0x1f7
#define IDE_DAT 0x1f0
#define IDE_BUSY 0x80
#define IDE_DRDY 0x40
#define IDE_DF   0x20
#define IDE_DRQ  0x08
#define IDE_ERR  0x01

void ide_init();