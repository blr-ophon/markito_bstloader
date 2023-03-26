section .asm
[BITS 32]

global ISR_wrapper_0
global IRQ_wrapper_21
global IRQ_wrapper_noIRQ


extern ISR_handler_0
extern IRQ_handler_21
extern IRQ_handler_noIRQ;

;TODO: ISR_wrapper should just receive address to a handler and call it. Same for IRQ.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Exceptions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ISR_wrapper_0:
    pushad              ;push all general purpose registers
    cld                 ;C code following the sysV ABI requires DF to be clear on function entry
    call ISR_handler_0
    popad
    iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Interrupts
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
IRQ_wrapper_21:
    cli
    pushad
    call IRQ_handler_21
    popad
    sti
    iret

IRQ_wrapper_noIRQ:
    cli
    pushad
    call IRQ_handler_noIRQ
    popad
    sti
    iret


