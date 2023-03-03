section .asm

global idt_load
idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]    ;first argument for this function
                        ;considering the return addresses are
                        ;all 4 bytes long
    lidt [ebx]
    pop ebp
    ret

