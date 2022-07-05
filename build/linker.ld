/* ENTRY is executed first, so we use _start */
ENTRY(_start)

SECTIONS
{
	/* Start at 64K, where the bootloader loads us */
	. = 0x10000;

	/* Executable code first */
	.text BLOCK(4K) : ALIGN(4K)
	{
		_start.o
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Initialized read/write data */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Uninitialized read/write data */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
