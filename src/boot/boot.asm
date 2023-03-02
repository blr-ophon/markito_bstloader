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

start_title:                ;Print title to test real mode
    mov si, title
    call print_loop
    mov si, rm_start_m
    call print_loop
    call keyboard_wait

_switch_to_protected:
    mov si, pm_start_m
    call print_loop
    cli                     ;Clear interrupts
    lgdt [gdt_descriptor]   ;load gdt descriptor
    mov eax, cr0            ;Set bit 0  of cr0
    or eax, 0x1
    mov cr0, eax
    ; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor) 
    ; to load CS with proper PM32 descriptor)
    jmp CODE_SEG:load_kernel

    
    jmp _end

_end:
    jmp $

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
    db "**Error reading sector**"

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
;                           PROTECTED-MODE CODE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32]

load_kernel:
    ;the 100 sectors of data after boot code are loaded at address 0x1M of RAM.
    mov eax, 1              ;start loading from sector 1
    mov cl, 100             ;100 sectors
    mov edi, 0x0100000      ;address 1M
    call ata_lba_read
    jmp CODE_SEG:0x0100000  ;jump to first line of kernel
    ;kernel must be linked with it's first line at 0x1M

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;EXPECTS:
;eax: Logical Block Adress (LBA) of sector
;cl: Number of sectors to read
;edi: address of buffer for the data read
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ata_lba_read:
    pushfd
    push eax
    push ebx
    push ecx
    push edx

    and eax, 0x0fffffff     ;isolate 28 bit address
    mov ebx, eax            ;Save full LBA to ebx

    mov edx, 0x01f6         ;edx <- Port to send... 
    shr eax, 24             ;... bits 24-27 of LBA
    or al, 11100000b        ; Set bit 6 in al for LBA mode 
                            ;and selects master drive
    out dx, al            

    mov eax, ebx            ;retrieve LBA address
    mov edx, 0x01f5         ;edx <- Port to send ...
    shr eax, 16             ;... bits 16-23 of LBA
    out dx, al             

    mov eax, ebx            ;retrieve LBA address
    mov edx, 0x01f4         ;edx <- Port to send ...
    shr eax, 8              ;... bits 8-15 of LBA
    out dx, al              

    mov eax, ebx            ;retrieve LBA address
    mov edx, 0x01f3         ;edx <- Port to send bits 0-7 of LBA
    out dx, al              
    
    mov eax, ebx            ;retrieve LBA address
    mov edx, 0x01f2         ;edx <- Port to send number of sectors
    mov al, cl
    out dx, al              

    mov edx, 0x1F7          ;edx <- Command port
    mov al, 0x20            ;Read with retry.
    out dx, al

next_sector:
    push ecx                ;save number of sectors

wait_sector_buffer:
    mov edx, 0x1F7          ;edx <- Status port
    in al, dx               
    test al, 0x8            ;bit 3 of status register, is set when...
                            ;the drive has PIO data to transfer or receive
    jz wait_sector_buffer

    mov ecx, 256            ;counter to be used by rep 256 16bit words
    mov edx, 0x1f0          ;port to receive data from
    rep insw                ;insw inputs 16bit word from edx port to edi address

    pop ecx                 ;retrieve number of sectors
    loop next_sector        ;same as dec ecx /jnz 

    pop edx
    pop ecx
    pop ebx
    pop eax
    popfd

    ret
    
title:
    db 0x0a,0x0d,"***** BLR-OPHON BOSTALOADER *****", 0x0a, 0x0d, 0x0a, 0x0d, 0
rm_start_m:
    db "Starting in real mode...", 0x0a, 0x0d, 0
pm_start_m:
    db "Switching to protected mode...", 0x0a, 0x0d, 0

times 510-($-$$) db 0       ;fill remaining spaces with 0
db 0x55, 0xaa               ;Boot signature
