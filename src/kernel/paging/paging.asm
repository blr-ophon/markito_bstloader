; To enable 32-bit paging, all that is needed is to load cr3
; with the address of the page directory and to set the paging
; (PG) and protection (PE) bits of cr0.


[BITS 32]
section .asm


global page_load_dir
global page_enable


;void page_load_dir(uint32_t *pd);
;moves page directory pd to cr3 register
page_load_dir:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
    mov cr3, eax

    pop ebp
    ret


;sets PG and PE bits of cr0
page_enable:
    push ebp
    mov ebp, esp

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    pop ebp
    ret

