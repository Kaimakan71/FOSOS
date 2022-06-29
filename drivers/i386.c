#include <i386.h>

static DTR gdtr;
static DTR idtr;
static Descriptor gdt[256];
static Descriptor idt[256];
static UInt16 gdtLength;

static void gdt_writeRawEntry(UInt16 selector, UInt32 low, UInt32 high) {
	UInt16 i = (selector & 0xfffc) >> 3;
	gdt[i].low = low;
	gdt[i].high = high;

	if(i > gdtLength) {
		gdtr.limit = (gdtLength + 1) * 8;
	}
}

void gdt_init() {
	gdtLength = 5;
	gdtr.base = gdt;
	gdtr.limit = (gdtLength * 8) - 1;

	gdt_writeRawEntry(0x0000, 0x00000000, 0x00000000);
	gdt_writeRawEntry(0x0008, 0x0000ffff, 0x00cf9a00);
	gdt_writeRawEntry(0x0010, 0x0000ffff, 0x00cf9200);
	gdt_writeRawEntry(0x0018, 0x0000ffff, 0x00cffa00);
	gdt_writeRawEntry(0x0020, 0x0000ffff, 0x00cff200);

	gdtr.base = gdt;
	gdtr.limit = (gdtLength * 8) - 1;
	asm("lgdt %0"::"m"(gdtr));
}

void idt_flush() {
	asm("lidt %0"::"m"(idtr));
}

void registerInterruptHandler(UInt8 vector, void (*f)()) {
	idt[vector].low = 0x00080000 | LSW((f));
	idt[vector].high = ((UInt32)(f) & 0xffff0000) | 0x8e00;
	idt_flush();
}

void registerUserInterruptHandler(UInt8 vector, void (*f)()) {
	idt[vector].low = 0x00080000 | LSW((f));
	idt[vector].high = ((UInt32)(f) & 0xffff0000) | 0xee00;
	idt_flush();
}

#define EH(i, msg) \
	static void _exception ## i () \
	{ \
		vga_setColor(0xa, 0x0); \
		print(msg"\n"); \
		dword cr0, cr2, cr3, cr4; \
		asm("movl %%cr0, %%eax":"=a"(cr0)); \
		asm("movl %%cr2, %%eax":"=a"(cr2)); \
		asm("movl %%cr3, %%eax":"=a"(cr3)); \
		asm("movl %%cr4, %%eax":"=a"(cr4)); \
		print("CR0=%x CR2=%x CR3=%x CR4=%x\n", cr0, cr2, cr3, cr4); \
		hang(); \
	}

EH(0, "Divide error")
EH(1, "Debug exception")
EH(2, "Unknown error")
EH(3, "Breakpoint")
EH(4, "Overflow")
EH(5, "Bounds check")
EH(6, "Invalid opcode")
EH(7, "Coprocessor not available")
EH(8, "Double fault")
EH(9, "Coprocessor segment overrun")
EH(10, "Invalid TSS")
EH(11, "Segment not present")
EH(12, "Stack exception")
EH(13, "General protection fault")
EH(14, "Page fault")
EH(15, "Unknown error")
EH(16, "Coprocessor error")

static void unimp_trap() {
	print("Unhandled IRQ");
	hang();
}

// 8259 spurious IRQ
// Not sure what it's for, so it gets ignored
extern void irq7_handler();
asm(
	".globl irq7_handler \n"
	"irq7_handler: \n"
	"    iret\n"
);

void idt_init() {
	idtr.base = idt;
	idtr.limit = 0x100 * 8;

	for(UInt8 i = 0xff; i > 0x10; --i) registerInterruptHandler(i, unimp_trap);

	registerInterruptHandler(0x00, _exception0);
	registerInterruptHandler(0x01, _exception1);
	registerInterruptHandler(0x02, _exception2);
	registerInterruptHandler(0x03, _exception3);
	registerInterruptHandler(0x04, _exception4);
	registerInterruptHandler(0x05, _exception5);
	registerInterruptHandler(0x06, _exception6);
	registerInterruptHandler(0x07, _exception7);
	registerInterruptHandler(0x08, _exception8);
	registerInterruptHandler(0x09, _exception9);
	registerInterruptHandler(0x0a, _exception10);
	registerInterruptHandler(0x0b, _exception11);
	registerInterruptHandler(0x0c, _exception12);
	registerInterruptHandler(0x0d, _exception13);
	registerInterruptHandler(0x0e, _exception14);
	registerInterruptHandler(0x0f, _exception15);
	registerInterruptHandler(0x10, _exception16);

	registerInterruptHandler(0x57, irq7_handler);

	idt_flush();
}