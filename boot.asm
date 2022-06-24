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

cld ; Clear the direction flag so string ops count up

; Set up a buffer for the kernel
mov bx, 0x1000
mov es, bx
xor bx, bx ; At 1MiB

mov ah, 2  ; Command 2 - read sectors
mov al, 72 ; 72 sectors (max allowed by bochs BIOS)
mov cx, 10 ; Track 0, sector 10
xor dx, dx ; Drive 0, head 0
int 0x13

; Check carry and return code for errors
jc diskError
cmp ah, 0
jne diskError

cli ; Disable interrupts

lgdt [gdt_descriptor] ; Load the GDT

; Set Protect Enable bit in control register 0
mov eax, cr0
or al, 1
mov cr0, eax

jmp 8:initProtected

diskErrorMsg: db "A disk read error occurred.", 0
diskError:
	mov si, diskErrorMsg
	mov ah, 0xe

	diskError.loop:
		mov al, [si]
		cmp al, 0
		je diskError.exit

		int 0x10
		inc si
		jmp diskError.loop
	diskError.exit:
		cli ; No interrupts
		hlt ; Hang :~(

gdt_start:
    dd 0, 0
    dd 0x0000ffff
    dd 0x00cf9a00
    dd 0x0000ffff
    dd 0x00cf9200
    dd 0, 0
    dd 0, 0
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

    mov esp, 0x7bff ; Set up a 32-bit stack

    jmp 0x10000 ; Jump to the kernel

	; If we somehow get back here, hang the computer
	cli
    hlt

times 510-($-$$) db 0 ; Make sure this is one sector (boot sector)
dw 0xaa55             ; Magic 'bootable' marker :~)