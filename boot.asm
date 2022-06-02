;-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+;
;                                                                              ;
;                            FOSOS i386 bootloader                             ;
;                                                                              ;
;                  Copyright (c) 2022, the FOSOS developers.                   ;
;                    SPDX-License-Identifier: BSD-2-Clause                     ;
;                                                                              ;
;-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+;
[bits 16]    ; We start in 16-bit real mode
[org 0x7c00] ; The BIOS loads us in to 7c00-7dff

jmp 0x0000:boot ; Jump to entry with CS=0

boot:
	; Switch to 80x25x16 text mode (clears screen)
	xor ah, ah
	mov al, 3
	int 0x10

	; Kernel code buffer setup
	xor ax, ax
	mov es, ax
	mov bx, 0x7e00 ; Right after boot sector

	; Load kernel into memory
	mov ah, 2
	mov al, 1 ; Read kernel code
	xor ch, ch
	mov dh, ch
	mov cl, 2
	int 0x13 ; Read!

	; Disk error handling
	jc diskError
	cmp ah, 0
	jne diskError

	cli ; Disable interrupts
	cld ; Clear direction flag so string ops count up

	lgdt [GDT_descriptor] ; Load Global Descriptor Table

	; Set Protected Enable bit in cr0
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp CODE_SEG:initProtected ; We are in protected mode, jump to the kernel

diskErrorMsg: db "Non-system disk or disk error", 0
diskError:
	mov si, diskErrorMsg
	mov ah, 0xe

	diskError.loop:
		mov al, [si]
		cmp al, 0
		je diskError.exit

		int 0x10
		add si, 1
		jmp diskError.loop
	diskError.exit:
		jmp $

GDT_start:
	; Null descriptor
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x00
	db 0x00
	db 0x00
GDT_code:
	; Code descriptor
	dw 0xffff
	dw 0x0000
	db 0x00
	db 0x9a
	db 0xcf
	db 0x00
GDT_data:
	; Data descriptor
	dw 0xffff
	dw 0x0000
	db 0x00
	db 0x92
	db 0xcf
	db 0x00
GDT_end:

; Like a handle to the GDT
GDT_descriptor:
	dw GDT_end - GDT_start - 1
	dd GDT_start

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

[bits 32] ; We have 32 bits now!
initProtected:
	; Segment setup
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Stack setup
	mov ebp, 0x7c00 ; Right below the bootloader
	mov esp, ebp

	jmp 0x7e00 ; Go to the kernel :~)

times 510-($-$$) db 0 ; Pad to 512 bytes (1 sector)
dw 0xaa55             ; Magic number, marks this as bootable