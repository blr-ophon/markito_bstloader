#include "paging/paging.h"

/*
 * Creates a page directory with all pages set with 'flags'.
 * Returns directory.
 */
struct page_dir *page_dir_init(uint8_t flags){
    //Directory with 1024 pdes (aka page tables)
    uint32_t *page_tables = kzalloc(sizeof(uint32_t) * PAGE_TABLE_TOTAL); 

    int offset = 0;
    for(int i = 0; i < PAGE_TABLE_TOTAL; i++){

        //pde with 1024 ptes (page table entries)
        uint32_t *pde = kzalloc(sizeof(uint32_t) * PTE_PER_TABLE);
        for(int j = 0; j < PTE_PER_TABLE; j++){
            //set pte in page table
            //address on last 20 bits, flags on first 12 (8 for 4Kib pages) bits
            pde[j] = (offset + (j * PAGE_SIZE)) | flags;
        }

        //set pde in page directory
        page_tables[i] = (uint32_t)pde | flags | PTE_FLAG_RW;

        //offset for next pde
        offset += PTE_PER_TABLE * PAGE_SIZE;
    }

    struct page_dir *page_dir = kzalloc(sizeof(struct page_dir));
    page_dir->page_tables = page_tables;
    
    return page_dir;
}
