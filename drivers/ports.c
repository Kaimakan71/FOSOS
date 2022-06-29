#include <ports.h>

byte inb(UInt16 port) {
	byte value;
	asm("inb %%dx, %%al":"=a"(value):"d"(port));
	return value;
}

word inw(UInt16 port) {
	word value;
	asm("inw %%dx, %%ax":"=a"(value):"d"(port));
	return value;
}

dword inl(UInt16 port) {
	dword value;
	asm("inl %%dx, %%eax":"=a"(value):"d"(port));
	return value;
}

void outb(UInt16 port, byte value) {
	asm("outb %%al, %%dx"::"d"(port), "a"(value));
}

void outw(UInt16 port, word value) {
	asm("outw %%ax, %%dx"::"d"(port), "a"(value));
}

void outl(UInt16 port, dword value) {
	asm("outl %%eax, %%dx"::"d"(port), "a"(value));
}

byte cmos_read(UInt8 index) {
	outb(0x70, index);
	return inb(0x71);
}

void cmos_write(UInt8 index, byte data) {
	outb(0x70, index);
	outb(0x71, data);
}