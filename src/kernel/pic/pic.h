#ifndef PIC_H
#define PIC_H

#include "io_routines/io_r.h"

#define MPIC_CTRL_PORT 0x20
#define MPIC_DATA_PORT 0x21
#define SPIC_CTRL_PORT 0xa0
#define SPIC_DATA_PORT 0xa1

/*
 * PIC required 4 initialization command words (ICW1-4), with 3 and 4 being optional.
 */

enum{
    ICW1_ICW4       = 0x01,         //set if ICW4 is needed before initialization (see datasheet)
    ICW1_SINGLE     = 0x02,         //single/cascade mode
    ICW1_INTERVAL   = 0x04,         //ignored on x86. Set to zero
    ICW1_LEVEL      = 0x08,         //edge/level triggered mode
    ICW1_INIT       = 0x10          //initialize PIC
} PIC_ICW1;

enum{
    ICW4_8086               = 0x01,         //(1)8086/8088 Mode or (0)MCS 80/85 MODE
    ICW4_AUTO_EOI           = 0x02,         //auto or normal end of interrupt
    ICW4_BUFFERED_SLAVE     = 0x08, //Not important for x86
    ICW4_BUFFERED_MASTER    = 0x0c, //Not important for x86 
    ICW4_FULLY_NESTED       = 0x10  //Not important for x86
} PIC_ICW4;

//OPERATION COMMAND WORDS
//after initialization, 8259 can accept interrupts and be modified via OCWs
//OCW1 deals with masks
enum{
    OCW2_EOI        = 0x20,         //End Of Interrupt
    OCW2_READ_IRR   = 0x0a,         //IRQ Request Register
    OCW2_READ_ISR   = 0x0a          //InService Register
} PIC_OCW2;

#endif
