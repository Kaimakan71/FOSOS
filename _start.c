extern void init();

extern void _start() {
	init();
	asm volatile("cli; hlt");
}