;-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+;
;                                                                              ;
;                            FOSOS i386 bootloader                             ;
;                                                                              ;
;                  Copyright (c) 2022, the FOSOS developers.                   ;
;                    SPDX-License-Identifier: BSD-2-Clause                     ;
;                                                                              ;
;-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+;
[bits 16]    ; We start in 16-bit real mode
[org 0x7c00] ; The BIOS loads us at 7c00

VIDEOMODE equ 0x101

cld ; Clear the direction flag so string ops count up

; Print a boot message
mov si, bootMsg
call print

; Set up a buffer for the kernel at 64K
mov bx, 0x1000
mov es, bx
xor bx, bx

mov ah, 0x02
mov al, 72
mov cx, 10
xor dh, dh
int 0x13

; Check carry and return code for errors
jc diskError
cmp ah, 0
jne diskError

; Switch to video mode
mov ax, 0x4f00
xor dx, dx
mov es, dx
mov di, 0xc000
mov [es:di], byte 'V'
mov [es:di+1], byte 'B'
mov [es:di+2], byte 'E'
mov [es:di+3], byte '2'
int 0x10
cmp ax, 0x004f
jne biosError
cmp [es:di], byte 'V'
jne biosError
cmp [es:di+1], byte 'E'
jne biosError
cmp [es:di+2], byte 'S'
jne biosError
cmp [es:di+3], byte 'A'
jne biosError

mov ax, 0x4f01
mov cx, VIDEOMODE | 0x4000
xor dx, dx
mov es, dx
mov di, 0x2000
int 0x10
cmp ax, 0x004f
jne biosError

mov ax, 0x4f02
mov bx, VIDEOMODE | 0x4000
int 0x10
cmp ax, 0x004f
jne biosError

cli ; Disable interrupts

; Enable A20 (8042)
call empty8042
mov al, 0xd1
out 0x64, al
call empty8042
mov al, 0xdf
out 0x60, al
call empty8042

lgdt [gdt_descriptor] ; Load the GDT

; Set Protect Enable bit in control register 0
mov eax, cr0
or al, 1
mov cr0, eax

jmp 8:initProtected

bootMsg: db "Starting FOSOS...", 13, 10, 0
diskErrorMsg: db "A disk read error occurred.", 13, 10, 0
biosErrorMsg: db "A BIOS error occurred.", 13, 10, 0

print:
	mov ah, 0x0e

	print_loop:
		mov al, [si]
		cmp al, 0
		je print_exit

		int 0x10
		inc si
		jmp print_loop
	print_exit:
		ret

diskError:
	mov si, diskErrorMsg
	call print

	cli ; No interrupts
	hlt ; Hang :~(

biosError:
	mov si, biosErrorMsg
	call print

	cli ; No interrupts
	hlt ; Hang :~(

empty8042:
	dw 0x00eb, 0x00eb
	in al, 0x64
	test al, 2
	jnz empty8042
	ret

gdt_start:
	dd 0, 0
	dd 0x0000ffff
	dd 0x00cf9a00
	dd 0x0000ffff
	dd 0x00cf9200
gdt_end:

; Like a handle to the GDT
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

[bits 32] ; We have 32 bits now!
initProtected:
	mov ax, 16
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov esp, 0x2000 ; Set up a 32-bit stack

	jmp 0x10000 ; Jump to the kernel

times 510-($-$$) db 0 ; Make sure this is one sector (boot sector)
dw 0xaa55             ; Magic 'bootable' marker :~)