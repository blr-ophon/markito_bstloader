#include "io_routines/irq.h"

#define PIC_OFFSET 0x20

void irq_init(void){
    PIC_init(PIC_OFFSET, PIC_OFFSET + 8);

    //TODO: register ISR handlers for each of the 16 irq lines

    //ior_enable_interrupts();
}

void IRQ_handler_noIRQ(void){
    ior_outb(0x20, 0x20);
}

void IRQ_handler_21(void){
    vgam3_print("Key pressed\n", MAIN_M3_COLOR);
    ior_outb(0x20, 0x20);   //Acknowledge

}
