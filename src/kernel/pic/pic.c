#include "pic/pic.h"

void PIC_init(void){
    ior_outb(MPIC_CTRL_PORT, ICW1_ICW4 | ICW1_INIT);
    ior_wait();
    ior_outb(SPIC_CTRL_PORT, ICW1_ICW4 | ICW1_INIT);
    ior_wait();
}
