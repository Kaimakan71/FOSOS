/**
 * FOSOS PCI device manager
 *
 * Copyright (c) 2022, the FOSOS developers.
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <pci.h>

UInt32 pci_getPCIIOAddress(PCIAddress addr, UInt8 field) {
	return 0x80000000 | (addr.bus << 16) | (addr.slot << 11) | (addr.function << 8) | (field & 0xfc);
}

byte pci_readFieldb(PCIAddress addr, UInt32 field) {
	outl(PCI_ADDRESS_PORT, getPCIIOAddress(addr, field));
	return inb(PCI_VALUE_PORT + (field & 3));
}

void pci_writeFieldb(PCIAddress addr, UInt32 field, byte data) {
	outl(PCI_ADDRESS_PORT, getPCIIOAddress(addr, field));
	outb(PCI_VALUE_PORT + (field & 3), data);
}

word pci_readFieldw(PCIAddress addr, UInt32 field) {
	outl(PCI_ADDRESS_PORT, getPCIIOAddress(addr, field));
	return inw(PCI_VALUE_PORT + (field & 2));
}

void pci_writeFieldw(PCIAddress addr, UInt32 field, word data) {
	outl(PCI_ADDRESS_PORT, getPCIIOAddress(addr, field));
	outw(PCI_VALUE_PORT + (field & 2), data);
}

dword pci_readFieldl(PCIAddress addr, UInt32 field) {
	outl(PCI_ADDRESS_PORT, getPCIIOAddress(addr, field));
	return inl(PCI_VALUE_PORT);
}

void pci_writeFieldl(PCIAddress addr, UInt32 field, dword data) {
	outl(PCI_ADDRESS_PORT, getPCIIOAddress(addr, field));
	outl(PCI_VALUE_PORT, data);
}

UInt16 pci_getType(PCIAddress addr) {
	return (pci_readFieldb(addr, PCI_CLASS) << 8) | pci_readFieldb(addr, PCI_SUBCLASS);
}

void pci_enumerateBus(int type, UInt8 bus, void(*callback)(PCIAddress, PCIID));

void pci_enumerateFunctions(int type, PCIAddress addr, void(*callback)(PCIAddress, PCIID)) {
	if(type == -1 || type == pci_getType(addr)) {
		PCIID id;
		id.vendorID = pci_readFieldw(addr, PCI_VENDOR_ID);
		id.deviceID = pci_readFieldw(addr, PCI_DEVICE_ID);
		callback(addr, id);
	}

	if(pci_getType(addr) == PCI_TYPE_BRIDGE) {
		UInt8 secondaryBus = pci_readFieldb(addr, PCI_SECONDARY_BUS);
		printf("[PCI] Found secondary bus %u\n", secondaryBus);
		ASSERT(secondaryBus != addr.bus);
		pci_enumerateBus(type, secondaryBus, callback);
	}
}

void pci_enumerateSlot(int type, UInt8 bus, UInt8 slot, void(*callback)(PCIAddress, PCIID)) {
	PCIAddress addr;
	addr.bus = bus;
	addr.slot = slot;

	if(pci_readFieldw(addr, PCI_VENDOR_ID) == PCI_NONE) return;

	pci_enumerateFunctions(type, addr, callback);

	if(!(pci_readFieldb(addr, PCI_HEADER_TYPE) & 0x80)) return;

	for(UInt8 function = 1; function < 8; function++) {
		PCIAddress address;
		address.bus = bus;
		address.slot = slot;
		address.function = function;
		if(pci_readFieldw(address, PCI_VENDOR_ID) != PCI_NONE) {
			pci_enumerateFunctions(type, address, callback);
		}
	}
}

void pci_enumerateBus(int type, UInt8 bus, void(*callback)(PCIAddress, PCIID)) {
	for(UInt8 slot = 0; slot < 32; slot++) {
		pci_enumerateSlot(type, bus, slot, callback);
	}
}

UInt8 pci_getInterruptLine(PCIAddress addr) {
	return pci_readFieldb(addr, PCI_INTERRUPT_LINE);
}

UInt32 pci_getBAR0(PCIAddress addr) { return pci_readFieldl(addr, PCI_BAR0); }
UInt32 pci_getBAR1(PCIAddress addr) { return pci_readFieldl(addr, PCI_BAR1); }
UInt32 pci_getBAR2(PCIAddress addr) { return pci_readFieldl(addr, PCI_BAR2); }
UInt32 pci_getBAR3(PCIAddress addr) { return pci_readFieldl(addr, PCI_BAR3); }
UInt32 pci_getBAR4(PCIAddress addr) { return pci_readFieldl(addr, PCI_BAR4); }
UInt32 pci_getBAR5(PCIAddress addr) { return pci_readFieldl(addr, PCI_BAR5); }

void pci_enableBusMastering(PCIAddress addr) {
	UInt16 value = pci_readFieldw(addr, PCI_COMMAND);
	value |= (1 << 2);
	value |= (1 << 0);
	pci_writeFieldw(addr, PCI_COMMAND, value);
}

void pci_enumerateAll(void(*callback)(PCIAddress, PCIID)) {
	// Single host controller
	PCIAddress addr;
	if((pci_readFieldb(addr, PCI_HEADER_TYPE) & 0x80) == 0) {
		pci_enumerateBus(-1, 0, callback);
		return;
	}

	// Multiple host controllers
	for(UInt8 function = 0; function < 8; function++) {
		PCIAddress address;
		address.bus = 0;
		address.slot = 0;
		address.function = function;
		if(pci_readFieldw(address, PCI_VENDOR_ID) == PCI_NONE) break;
		pci_enumerateBus(-1, function, callback);
	}
}