ORG 0x7c00
[BITS 16]
;Offsets for code and data GDTs
CODE_SEG equ code_descriptor - gdt_start       
DATA_SEG equ data_descriptor - gdt_start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                           REAL-MODE CODE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

start_title:                ;Print title to test real mode
    mov si, title

    push si
    call print_loop
    pop si
    call keyboard_wait

    mov si, markito

    push si
    call print_loop
    pop si

_switch_to_protected:
    cli                     ;Clear interrupts
    lgdt [gdt_descriptor]   ;load gdt descriptor
    mov eax, cr0            ;Set bit 0  of cr0
    or eax, 0x1
    mov cr0, eax
    ; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor) 
    ; to load CS with proper PM32 descriptor)
    jmp CODE_SEG:load32

_end:
    jmp $

    jmp _end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                           REAL-MODE SUBROUTINES
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
    null_descriptor:
        dd 0x00000000           
        dd 0x00000000          

    code_descriptor:        ;Offset + 8 ;CS
        dw 0xffff           ;Limit_1
        dw 0x0000           ;Base_1 
        db 0x00             ;Base_2 
        db 0x9a             ;Access Byte
        db 11001111b        ;Limit_2(1111) and Flags(1100)
        db 0x00             ;Base3

        ;Limit: 0xfffff, 4gb of addressable unit
        ;Base: 0x00000000, Segment begins at 0x0000
        ;Access Byte: |valid segment | ring 0 | code/data type | executable |
        ;             | executed only from ring 0 | readable | 0 (CPU will handle) |
        ;Flags: | Limit in 4kiB blocks | 32-bit protected segment | non 64-bit segment |

    data_descriptor:        ;Offset + 16 ;DS, SS, ES, FS, GS
        dw 0xffff           ;Limit_1
        dw 0x0000           ;Base_1
        db 0x00             ;Base_2 
        db 0x92             ;Access Byte
        db 11001111b        ;Limit_2(1111) and Flags(1100)
        db 0x00             ;Base_3

        ;Limit: 0xfffff, 4gb of addressable unit
        ;Base: 0x00000000, Segment begins at 0x0000
        ;Access Byte: |valid segment | ring 0 | code/data type | non-executable |
        ;             | segment grows UP | writable | 0 (CPU will handle) |
        ;Flags: | Limit in 4kiB blocks | 32-bit protected segment | non 64-bit segment |

    gdt_descriptor:                         ;Offset + 24
        dw gdt_descriptor - gdt_start - 1   ;size
        dd gdt_start                        ;offset

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                       PROTECTED MODE CODE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[BITS 32]
load32:
    ;set segmented registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ;set stack
    mov ebp, 0x00200000
    mov esp, ebp

    mov esi, title
    mov edi, 0xb8000
    call print_loop_32
    jmp $

times 510-($-$$) db 0       ;fill remaining spaces with 0
db 0x55, 0xaa               ;Boot signature

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                       PROTECTED-MODE SUBROUTINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

print_loop_32:
    mov ah, 0x0f            
    lodsb                   ;al <- [ds:si], si++
    mov [edi], ax
    inc edi
    test al,al              ;[si] == 0?
    jne print_loop_32
    ret

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
