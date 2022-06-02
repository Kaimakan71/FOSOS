// Entry point. Must be placed first
extern void _start() {
	// Completely stop everything
	asm volatile("cli; hlt");
}