#include "idt.h"

IDT_Gate IDT[INTERRUPTS_TOTAL];         //Table of gate decriptors
IDT_Descriptor idt_descriptor;                                    

extern void idt_load(IDT_Descriptor *descriptor);   //call asm code to load lidt
                                                    //
extern void ISR_wrapper_0();

extern void IRQ_wrapper_21();
extern void IRQ_wrapper_noIRQ();

void ISR_handler_0(void){
    vgam3_print("ERROR: zero division\n", MAIN_M3_COLOR);
}


void idt_set(int i, void *adr){
    //set entry i in Descriptor Table to adr
    IDT_Gate *entry = &IDT[i];      //get entry pointer
    entry->offset_l = ((uint32_t)adr) & 0x0000ffff;  
    entry->segment_selector = KERNEL_CODE_SELECTOR;
    entry->reserved = 0;
    entry->flags = (IDT_FLAGS) IDT_FLAG_GATE_INT32;
    entry->flags |= (IDT_FLAGS) IDT_FLAG_RING3;
    entry->flags |= (IDT_FLAGS) IDT_FLAG_PRESENT;
    entry->offset_h = (((uint32_t)adr) & 0xffff0000) >> 16;
}

void idt_init(void){
    ior_disable_interrupts();
    irq_init();
    n_memset(IDT, 0, sizeof(IDT));
    for(int i = 0; i < INTERRUPTS_TOTAL; i++){
        idt_set(i, IRQ_wrapper_noIRQ);
    }

    //set ISRs
    idt_set(0, ISR_wrapper_0);
    idt_set(0x21, IRQ_wrapper_21);

    //Load descriptor in IDT register
    idt_descriptor.size = sizeof(IDT) - 1;      
    idt_descriptor.offset = (uint32_t) IDT;
    idt_load(&idt_descriptor);
    ior_enable_interrupts();
}

