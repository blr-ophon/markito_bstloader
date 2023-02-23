ORG 0x7c00
BITS 16

_start:
    jmp short _setup
    nop
    times 33 db 0           ;Clear BPB (BIOS Parameter Block)
    jmp 0x00:_setup        ;cs <- 0x7c0

_setup:
    cli                     ;Clear interrupts 
    ;Load segmented registers
    mov ax, 0x0            
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov bp, 0x1200          ;Stack base pointer [ss:bp]
    mov sp, bp              ;Empty stack
    sti                     ;Enable interrupts

    ;Read other sectors
    mov ah, 2               ;INT 13/AH = 0x: Read sector into memory
    mov bx, 0x7e00           ;Start adress
    mov al, 2               ;number of sectors
    mov ch, 0               ;Cylinder number
    mov cl, 2               ;Sector number
    mov dh, 0               ;Head number
    int 0x13                ;Read sector 
    jc read_s_error

start_title:
    mov si, title

    push si
    call print_loop
    pop si
    call keyboard_wait

    mov si, markito

    push si
    call print_loop
    pop si

_end:
    jmp $

    jmp _end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                               SUBROUTINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

print_loop:
    mov ah, 0x0e            ;INT 10/AH = 0x0: tty mode
    lodsb                   ;al <- [ds:si], si++
    int 0x10                ;Print to tty
    test al,al              ;[si] == 0?
    jne print_loop
    ret

keyboard_wait:
    mov ah, 0               ;INT 16/AH = 0x0: Get keystroke
    int 0x16                ;Get keystroke
    ret

read_s_error:
    mov si, _read_s_error_m
    call print_loop
    jmp _end
    
_read_s_error_m:
    db "Error reading sector"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                       GLOBAL DESCRIPTOR TABLE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;Pointed to by value in gdtr register
gdt_start:                  ;0ffset + 0
    dd 0x00000000           
    dd 0x00000000          

gdt_code:                   ;Offset + 8 ;CS
    ;Limit: 0xfffff, 4gb of addressable unit
    ;Base: 0x00000000, Segment begins at 0x0000
    ;Access Byte: |valid segment | ring 0 | code/data type | executable |
    ;             | executed only from ring 0 | readable | 0 (CPU will handle) |
    ;Flags: | Limit in 4kiB blocks | 32-bit protected segment | non 64-bit segment |
    dw 0xffff               ;Limit_1
    dw 0x0000               ;Base_1 
    db 0x00                 ;Base_2 
    db 0x9a                 ;Access Byte
    db 11001111b            ;Limit_2(1111) and Flags(1100)
    db 0x00                 ;Base3

gdt_data:                   ;Offset + 16 ;DS, SS, ES, FS, GS
    ;Limit: 0xfffff, 4gb of addressable unit
    ;Base: 0x00000000, Segment begins at 0x0000
    ;Access Byte: |valid segment | ring 0 | code/data type | non-executable |
    ;             | segment grows UP | writable | 0 (CPU will handle) |
    ;Flags: | Limit in 4kiB blocks | 32-bit protected segment | non 64-bit segment |
    dw 0xffff               ;Limit_1
    dw 0x0000               ;Base_1
    db 0x00                 ;Base_2 
    db 0x92                 ;Access Byte
    db 11001111b            ;Limit_2(1111) and Flags(1100)
    db 0x00                 ;Base_3

gdt_end:                    ;Offset + 24

gdt_descriptor:             
    dw gdt_end - gdt_start - 1  ;size
    dd gdt_start                ;offset

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

times 510-($-$$) db 0       ;fill remaining spaces with 0
db 0x55, 0xaa               ;Boot signature

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                       0x200 offset sector
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
title:
    db "***** BLR-OPHON BOSTALOADER VERSION 1.0 *****", 0x0a, 0x0d, 0
markito:
    db "        ####            ",0xa,0xd
    db "      ##########        ",0xa,0xd
    db "  ###################   ",0xa,0xd
    db "  ####################  ",0xa,0xd
    db " #####################  ",0xa,0xd
    db "##########  ##########  ",0xa,0xd
    db "#######       ########  ",0xa,0xd
    db "####             #####  ",0xa,0xd
    db " ##              #####  ",0xa,0xd
    db "  #               ####  ",0xa,0xd
    db "           ###########  ",0xa,0xd
    db "  #################    #",0xa,0xd
    db "  #####  ##########  # #",0xa,0xd
    db "  #####  #########    # ",0xa,0xd
    db "   ## #               # ",0xa,0xd
    db "     #                  ",0xa,0xd
    db "  #                     ",0xa,0xd
    db "  #                     ",0xa,0xd
    db "                #       ",0xa,0xd
    db "    # #####  ##      #  ",0xa,0xd
    db "      ##    ##          ",0xa,0xd
    db "      ######       #    ",0xa,0xd
    db "        ###             ",0xa,0xd 
    db "       #                ",0xa,0xd
    db "         #####          ",0xa,0xd,0
