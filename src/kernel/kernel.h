#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include "vgam3.h"
#include "isr/idt.h"
#include "memory/kheap.h"
#include "paging/paging.h"
#include "fs/pathparser.h"
#include "disk/diskstream.h"

void screen_startup(void);

void kernel_main(void);

#endif
