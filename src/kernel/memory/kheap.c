#include "kheap.h"

struct heap kernel_heap;
struct heap_entry_table kernel_het;

void kheap_init(void){
    kernel_het.entries = (HET_BLOCK*) HEAP_TABLE_ADDR;
    kernel_het.total = HET_TOTAL;

    void *end = (void*)(HEAP_ADDR + HEAP_SIZE);
    int res = heap_create(&kernel_heap, (void*)HEAP_ADDR, end, &kernel_het);
    if(res < 0){
        //TODO: Panic
    }
}
