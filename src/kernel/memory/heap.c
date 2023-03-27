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

out:
    return res;
}

static uint32_t blockAlign(size_t n){
    //return space that must be allocated in bytes, in multiples
    //of HEAP_BLOCK_SIZE
    if(n % HEAP_BLOCK_SIZE == 0){
        return n;
    }
    return (n/HEAP_BLOCK_SIZE + 1) * HEAP_BLOCK_SIZE;
}


void *heap_malloc(struct heap_des *heapd, size_t n){
    size_t blocks_size = blockAlign(n);
    uint32_t wanted_blocks = blocks_size/HEAP_BLOCK_SIZE;
    if(wanted_blocks == 0){
        return NULL;
    }


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
    table->entries[start_index + j] |= HET_BLOCK_IS_FIRST; 
    for(; j < wanted_blocks - 1; j++){
        table->entries[start_index + j] |= HET_BLOCK_TAKEN; 
        table->entries[start_index + j] |= HET_BLOCK_HAS_NEXT; 
    }
    table->entries[start_index + j] |= HET_BLOCK_TAKEN; 


    return (void*) start_address;
}

void heap_free(struct heap_des *heapd, void *adr){
    //Validate adr
    if(!heap_isAligned(adr)){
        return; 
    }
    struct heap_entry_table *table = heapd->table;

    //get entry table index for adr
    int entry_index = (((uint32_t) adr) - (uint32_t) heapd->addr)/HEAP_BLOCK_SIZE;
    
    size_t i = entry_index;
    table->entries[i] &= ~(HET_BLOCK_IS_FIRST);
    while(table->entries[i] & HET_BLOCK_HAS_NEXT){
        table->entries[i] &= ~(HET_BLOCK_HAS_NEXT);
        table->entries[i] &= ~(HET_BLOCK_TAKEN);
        i++;
    }
    table->entries[i] &= ~(HET_BLOCK_TAKEN);
}
