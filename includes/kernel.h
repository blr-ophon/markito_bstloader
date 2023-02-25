#ifndef KERNEL_H
#define KERNEL_H

#define VGA_MODE3_W 80
#define VGA_MODE3_H 25
#define MAIN_M3_COLOR 0x1f

#include <stdint.h>

void screen_init(uint8_t color);

void vgam3_draw(char *c, int x, int y, uint8_t color);

void vgam3_print(char *c, uint8_t color);

void kernel_main(void);

#endif
