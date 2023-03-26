section .asm
global ior_inb
global ior_inw
global ior_outb
global ior_outw
global ior_enable_interrupts
global ior_disable_interrupts

ior_inb:
    ;after new stack frame is set, parameters are accessed by 
    ;incrementing ebp and local variables by subtracting esp
    push ebp            ;start stack frame/ ENTER
    mov ebp, esp
    xor eax, eax
    ;ebp has previous ebp, ebp+4 has return address, ebp+8 has first parameter
    mov dx, [ebp+8]    
    in al, dx

    mov esp, ebp        ;deallocate local variables
    pop ebp             ;cleanup stack frame/ LEAVE
    ret

ior_inw:
    ;after new stack frame is set, parameters are accessed by 
    ;incrementing ebp and local variables by subtracting esp
    push ebp            ;start stack frame/ ENTER
    mov ebp, esp
    xor eax, eax
    ;ebp has previous ebp, ebp+4 has return address, ebp+8 has first parameter
    mov dx, [ebp+8]    
    in ax, dx

    mov esp, ebp        ;deallocate local variables
    pop ebp             ;cleanup stack frame/ LEAVE
    ret

ior_outb:
    ;after new stack frame is set, parameters are accessed by 
    ;incrementing ebp and local variables by subtracting esp
    push ebp            ;start stack frame/ ENTER
    mov ebp, esp
    
    ;ebp has previous ebp, ebp+4 has return address, ebp+8 has first parameter
    mov eax, [ebp+12]   ;second parameter (IA-32)
    mov dx, [ebp+8]    
    out dx, al

    mov esp, ebp        ;deallocate local variables
    pop ebp             ;cleanup stack frame/ LEAVE
    ret

ior_outw:
    ;after new stack frame is set, parameters are accessed by 
    ;incrementing ebp and local variables by subtracting esp
    push ebp            ;start stack frame/ ENTER
    mov ebp, esp
    
    ;ebp has previous ebp, ebp+4 has return address, ebp+8 has first parameter
    mov eax, [ebp+12]   ;second parameter (IA-32)
    mov dx, [ebp+8]    
    out dx, ax

    mov esp, ebp        ;deallocate local variables
    pop ebp             ;cleanup stack frame/ LEAVE
    ret

ior_enable_interrupts:
    sti
    ret

ior_disable_interrupts:
    cli 
    ret
