#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "config.h"
#include "mem.h"
#include "qosclib.h"

typedef struct {
    uint16_t offset_l;              //address of Interrupt Service Routine
    uint16_t segment_selector;      //Point to valid code segment in GDT
    uint8_t reserved;               //ignored
    uint8_t flags;                  //Gate type, CPU privilege and Present bit.
    uint16_t offset_h;              //address of Interrupt Service Routine
}__attribute__((packed)) IDT_Gate;

typedef struct {
    uint16_t size;                  //size of the IDT with all it's entries
    uint32_t offset;                //address of the IDT
}__attribute__((packed)) IDT_Descriptor;

typedef enum {
    IDT_FLAG_GATE_TASK          = 0x5,
    IDT_FLAG_GATE_INT           = 0x6,
    IDT_FLAG_GATE_TRAP          = 0x7,
    IDT_FLAG_GATE_INT32         = 0xe,
    IDT_FLAG_GATE_TRAP32        = 0xf,

    IDT_FLAG_RING0              = (0 << 5),
    IDT_FLAG_RING1              = (1 << 5),
    IDT_FLAG_RING2              = (2 << 5),
    IDT_FLAG_RING3              = (3 << 5),

    IDT_FLAG_PRESENT            = 0x80
} IDT_FLAGS;

void idt_init(void);

void idt_set(int i, void *adr);

void ISR_handler_0(void);

#endif
