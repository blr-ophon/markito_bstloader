#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>
#include "config.h"
#include "status.h"
#include "mem.h"

//HET = Heap Entry Table
#define HET_BLOCK_TAKEN     0x01
#define HET_BLOCK_FREE      0x00
#define HET_BLOCK_HAS_NEXT  (0x01 << 7)
#define HET_BLOCK_IS_FREE   (0x01 << 6)

typedef unsigned char HET_BLOCK;

struct heap_entry_table{
    HET_BLOCK *entries;
    size_t total;
};

struct heap{
    struct heap_entry_table *table;
    void *addr;
};

int heap_create(struct heap *heap, void *ptr, void *end, struct heap_entry_table *table);

#endif
