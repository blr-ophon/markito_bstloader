ORG 0
BITS 16

_start:
    jmp short _setup
    nop
    times 33 db 0           ;Clear BPB (BIOS Parameter Block)
    jmp 0x7c0:_setup        ;cs <- 0x7c0

_setup:
    ;Load segmented registers
    cli                     ;Clear interrupts 
    mov ax, 0x7c0
    mov es, ax
    mov ds, ax
    mov ax, 0x00
    mov ss, ax
    mov bp, 0x1200          ;Stack base pointer
    mov sp, bp              ;Empty stack
    sti                     ;Enable interrupts

    ;Read other sectors
    mov [BOOT_DISK], dl
    mov bx, 0x200          ;Start adress
    mov ah, 2               
    mov al, 2               ;number of sectors
    mov ch, 0               ;Cylinder number
    mov cl, 2               ;Sector number
    mov dh, 0               ;Head number
    mov dl, [BOOT_DISK]
    int 0x13                ;Read sectors 

_main:
    mov si, _title

    push si
    call _print_loop
    pop si
    call _keyboard_wait


    mov si, _markito

    push si
    call _print_loop
    pop si

    jmp _end

_print_loop:
    mov ah, 0x0e            ;teletype mode
    lodsb                   ;al <- [ds:si], si++
    int 0x10                ;call BIOS interrupt 0x10 for print
    test al,al              ;[si] == 0?
    jne _print_loop
    ret

_keyboard_wait:
    mov ah, 0
    int 0x16
    ret

_end:
    jmp $                   ;infinite loop 
BOOT_DISK: db 0
times 510-($-$$) db 0       ;fill remaining spaces with 0

db 0x55, 0xaa               ;Boot signature

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   0x200 (512 bytes from previous code)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_title:
    db "***** BLR-OPHON BOSTALOADER VERSION 1.0 *****", 0x0a, 0x0d, 0
_markito:
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
