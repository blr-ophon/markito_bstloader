#include "heap.h"


static int heap_isAligned(void *ptr){
    //Check if address is a multiple of HEAP_BLOCK_SIZE
    return (((uint32_t) ptr) % HEAP_BLOCK_SIZE) == 0;
}

static int HET_validate(void *ptr, void *end, struct heap_entry_table *table){
    //Verify each block from start to end has an entry in table
    int res = 0;
    size_t table_size = (size_t) (end - ptr);
    size_t blocks_n = table_size/HEAP_BLOCK_SIZE;
    if(blocks_n != table->total){
        res = -EINVARG;
    }
out:
    return res;
}

int heap_create(struct heap_des *heapd, void *ptr, void *end, struct heap_entry_table *table){
    //Fill heap descriptor with a pointer to an entry table and offset addresses for start and end
    int res = 0;
    if(!heap_isAligned(ptr) || !heap_isAligned(end)){
        res = -EINVARG;
        goto out;
    }
    n_memset(heapd, 0, sizeof(struct heap_des));
    heapd->addr = ptr;
    heapd->table = table;

    res = HET_validate(ptr, end, table);
    if(res < 0){
        res= -EINVARG;
        goto out;
    }

    n_memset(table, HET_BLOCK_FREE, HET_TOTAL);

out:
    return res;
}

static uint32_t blockAlign(uint32_t adr){
    if(adr % HEAP_BLOCK_SIZE){
        return adr;
    }
    return (adr/HEAP_BLOCK_SIZE + 1) * HEAP_BLOCK_SIZE;
}


void *n_malloc(struct heap_des *heapd, size_t size){
    size_t blocks_size = blockAlign(size);
    uint32_t wanted_blocks = blocks_size/HEAP_BLOCK_SIZE;


    //Find free blocks that can be allocated
    struct heap_entry_table *table = heapd->table;
    
    int free_blocks = 0;
    size_t i = 0;
    for(; i < heapd->table->total; i++){
        if(table->entries[i] & HET_BLOCK_TAKEN){
            free_blocks = 0;
            continue;
        }
        free_blocks++;

        if(free_blocks == wanted_blocks){
            break;
        }
    }

    if(free_blocks != wanted_blocks){
        //No space to allocate
        return NULL;
    }


    //Get address
    int start_index = i - wanted_blocks + 1;
    uint32_t start_address = ((uint32_t) heapd->addr) + (uint32_t)start_index*HEAP_BLOCK_SIZE;


    //Mark allocated blocks as taken
    size_t j = 0;
    for(; j < wanted_blocks - 1; j++){
        table->entries[start_index + j] |= HET_BLOCK_TAKEN; 
        table->entries[start_index + j] |= HET_BLOCK_HAS_NEXT; 
    }
    table->entries[start_index + j] |= HET_BLOCK_TAKEN; 


    return (void*) start_address;
}

void n_free(struct heap_des *heapd, size_t size){
}
