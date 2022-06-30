/* ENTRY is executed first, so we use _start */
ENTRY(_start)

SECTIONS
{
	/* Start at 64K, where the bootloader loads us */
	. = 0x10000;

	/* Executable code first */
	.text :
	{
		_start.o
		*(.text)
	}

	/* Read-only data */
	.rodata :
	{
		*(.rodata)
	}

	/* Initialized read/write data */
	.data :
	{
		*(.data)
	}

	/* Uninitialized read/write data and stack */
	.bss :
	{
		*(COMMON)
		*(.bss)
		*(.stack)
	}

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}
}
