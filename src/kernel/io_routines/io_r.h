#ifndef IO_R_H
#define IO_R_H

#include <stdint.h>

uint8_t ior_inb(uint16_t port);

uint8_t ior_inw(uint16_t port);

void ior_outb(uint16_t port, uint8_t val);

void ior_outbw(uint16_t port, uint8_t val);

#endif
