[BITS 32]

CODE_SEG equ 0x08
DATA_SEG equ 0x10

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
    
    ;enable A20 line using 'fast a20 gate'
    in al, 0x92
    or al, 2
    out 0x92, al

