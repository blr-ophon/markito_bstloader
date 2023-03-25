#include "pic/pic.h"

void PIC_init(uint8_t Mpic_offset, uint8_t Spic_offset){
    //ICW1
    ior_outb(MPIC_CTRL_PORT, ICW1_ICW4 | ICW1_INIT);
    ior_wait();
    ior_outb(SPIC_CTRL_PORT, ICW1_ICW4 | ICW1_INIT);
    ior_wait();

    //ICW2
    ior_outb(MPIC_DATA_PORT, Mpic_offset); 
    ior_wait();
    ior_outb(SPIC_DATA_PORT, Spic_offset); 
    ior_wait();

    //ICW3
    ior_outb(MPIC_DATA_PORT, 0x4);      //slave at IRQ2
    ior_wait();
    ior_outb(SPIC_DATA_PORT, 0x2);      //cascade identity
    ior_wait();

    //ICW4
    ior_outb(MPIC_DATA_PORT, ICW4_8086);      //8086 mode
    ior_wait();
    ior_outb(SPIC_DATA_PORT, ICW4_8086);      //8086 mode
    ior_wait();

    //clear 
    ior_outb(MPIC_DATA_PORT, 0);      //8086 mode
    ior_wait();
    ior_outb(SPIC_DATA_PORT, 0);      //8086 mode
    ior_wait();

}


void PIC_Mask(int irq_x){
    //Masks(disable) irqx 
    //Reads mask register from data port, sets the corresponding
    //mask bit and writes it back

    if(irq_x < 8){ //Master PIC
        uint8_t mask = ior_inb(MPIC_DATA_PORT);
        ior_outb(MPIC_DATA_PORT, mask | (1 << irq_x));
    }else{ //Slave PIC
        irq_x -= 8;     
        uint8_t mask = ior_inb(SPIC_DATA_PORT);
        ior_outb(SPIC_DATA_PORT, mask | (1 << irq_x));
    }
}

void PIC_unMask(int irq_x){
    //unmasks(enable) irqx 
    //Reads mask register from data port, clear the corresponding
    //mask bit and writes it back

    if(irq_x < 8){ //Master PIC
        uint8_t mask = ior_inb(MPIC_DATA_PORT);
        ior_outb(MPIC_DATA_PORT, mask & ~(1 << irq_x));
    }else{ //Slave PIC
        irq_x -= 8;     
        uint8_t mask = ior_inb(SPIC_DATA_PORT);
        ior_outb(SPIC_DATA_PORT, mask & ~(1 << irq_x));
    }
}

void PIC_Disable(void){
    //If using APIC instead of cascaded 8259 PICs
    ior_outb(MPIC_DATA_PORT, 0xff);
    ior_wait();
    ior_outb(SPIC_DATA_PORT, 0xff);
    ior_wait();
}

void PIC_Send_EOI(int irq){
    if(irq >= 8){
        ior_outb(SPIC_CTRL_PORT, OCW2_EOI);
    }
    ior_outb(MPIC_CTRL_PORT, OCW2_EOI);
}

uint16_t PIC_readIRR(void){
    //Send READ_IRR command word, read lower and high byte from ctrl port
    ior_outb(MPIC_CTRL_PORT, OCW2_READ_IRR);
    ior_wait();
    ior_outb(SPIC_CTRL_PORT, OCW2_READ_IRR);
    ior_wait();
    //litle endian
    return ior_inb(SPIC_CTRL_PORT) | (ior_inb(SPIC_CTRL_PORT) << 8);
}

uint16_t PIC_readISR(void){
    //Send READ_ISR command word, read lower and high byte from ctrl port
    ior_outb(MPIC_CTRL_PORT, OCW2_READ_ISR);
    ior_wait();
    ior_outb(SPIC_CTRL_PORT, OCW2_READ_ISR);
    ior_wait();
    //litle endian
    return ior_inb(SPIC_CTRL_PORT) | (ior_inb(SPIC_CTRL_PORT) << 8);
}
