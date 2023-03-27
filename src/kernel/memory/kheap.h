#ifndef KHEAP_H
#define KHEAP_H

#include "config.h"
#include "heap.h"

void kheap_init(void);

void *kmalloc(size_t n);

void kfree(void *adr);

#endif
