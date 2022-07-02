/**
 * FOSOS PCI device manager
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <types.h>
#include <assert.h>
#include <vga.h>
#include <ports.h>

#define PCI_HEADER_TYPE_DEVICE 0
#define PCI_HEADER_TYPE_BRIDGE 1

#define PCI_VENDOR_ID   0x00
#define PCI_DEVICE_ID   0x02
#define PCI_REVISION_ID 0x08
#define PCI_PROG_IF     0x09
#define PCI_CLASS       0x0b
#define PCI_SUBCLASS    0x0a
#define PCI_HEADER_TYPE 0x0e

#define PCI_COMMAND         0x04
#define PCI_STATUS          0x06
#define PCI_CACHE_LINE_SIZE 0x0c
#define PCI_LATENCY_TIMER   0x0d
#define PCI_BIST            0x0f
#define PCI_BAR0            0x10
#define PCI_BAR1            0x14
#define PCI_BAR2            0x18
#define PCI_BAR3            0x1C
#define PCI_BAR4            0x20
#define PCI_BAR5            0x24

#define PCI_INTERRUPT_LINE 0x3C
#define PCI_SECONDARY_BUS  0x19
#define PCI_TYPE_BRIDGE    0x0604
#define PCI_ADDRESS_PORT   0xCF8
#define PCI_VALUE_PORT     0xCFC
#define PCI_NONE           0xFFFF

typedef struct {
	UInt16 vendorID;
	UInt16 deviceID;
} PCIID;

typedef struct {
	UInt8 bus;
	UInt8 slot;
	UInt8 function;
} PCIAddress;

UInt32 getPCIIOAddress(PCIAddress addr, UInt8 field);
UInt8 pci_getInterruptLine(PCIAddress addr);
UInt32 pci_getBAR0(PCIAddress addr);
UInt32 pci_getBAR1(PCIAddress addr);
UInt32 pci_getBAR2(PCIAddress addr);
UInt32 pci_getBAR3(PCIAddress addr);
UInt32 pci_getBAR4(PCIAddress addr);
UInt32 pci_getBAR5(PCIAddress addr);
void pci_enableBusMastering(PCIAddress addr);
void pci_enumerateAll(void(*callback)(PCIAddress, PCIID));