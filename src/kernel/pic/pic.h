#ifndef PIC_H
#define PIC_H

#include "io_routines/io_r.h"

#define MPIC_CTRL_PORT 0x20
#define MPIC_DATA_PORT 0x21
#define SPIC_CTRL_PORT 0xa0
#define SPIC_DATA_PORT 0xa1

enum{
    ICW1_ICW4       = 0x01,         //set if ICW4 is needed before initialization (see datasheet)
    ICW1_SINGLE     = 0x02,         //single/cascade mode
    ICW1_INTERVAL   = 0x04,         //ignored on x86. Set to zero
    ICW1_LEVEL      = 0x08,         //edge/level triggered mode
    ICW1_INIT       = 0x10          //initialize PIC
} PIC_ICW1;

#endif
