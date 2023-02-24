[BITS 32]

global _start
extern kernel_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
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
    
    ;enable A20 line using 'fast a20 gate'
    in al, 0x92
    or al, 2
    out 0x92, al
    
    ;switch to C compiled code section
    call kernel_start

    jmp $

times 512-($-$$) db 0       ;to keep C compiled code aligned
