#include "heap.h"


static int heap_isAligned(void *ptr){
    return (((uint32_t) ptr) % HEAP_BLOCK_SIZE) == 0;
}

static int HET_validate(void *ptr, void *end, struct heap_entry_table *table){
    int res = 0;
out:
    return res;
}

int heap_create(struct heap *heap, void *ptr, void *end, struct heap_entry_table *table){
    int res = 0;
    if(!heap_isAligned(ptr) || !heap_isAligned(end)){
        res = -EINVARG;
        goto out;
    }
    n_memset(heap, 0, sizeof(struct heap));
    heap->addr = ptr;
    heap->table = table;

out:
    return res;
}
