#include "kheap.h"

struct heap_des kernel_heap;
struct heap_entry_table kernel_het;

void kheap_init(void){
    kernel_het.entries = (HET_BLOCK*) HEAP_TABLE_ADDR;
    kernel_het.total = HET_TOTAL;

    //Clear all entries
    for(int i = 0; i < HET_TOTAL; i++){
        kernel_het.entries[i] = 0;
    }

    void *end = (void*)(HEAP_ADDR + HEAP_SIZE);
    int res = heap_create(&kernel_heap, (void*)HEAP_ADDR, end, &kernel_het);
    if(res < 0){
        //TODO: Panic
    }
}

void *kmalloc(size_t n){
    return heap_malloc(&kernel_heap, n);
}

void *kzalloc(size_t n){
    void *ptr = kmalloc(n);
    n_memset(ptr, 0, n);
    return ptr;
}

void kfree(void *adr){
    heap_free(&kernel_heap, adr);
}
