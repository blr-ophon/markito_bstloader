section .asm

global ISR_wrapper_0

extern ISR_handler_0

;TODO: ISR_wrapper should just receive address to a handler and call it.
ISR_wrapper_0:
    pushad              ;push all general purpose registers
    cld                 ;C code following the sysV ABI requires DF to be clear on function entry
    call ISR_handler_0
    popad
    iret
    

