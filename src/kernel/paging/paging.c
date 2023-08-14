#include "paging/paging.h"

uint32_t *current_pd = NULL;


/*
 * Creates a page directory with all pages set with 'flags' and RW flags
 * Returns directory.
 */
struct page_dir *page_dir_init(uint8_t flags){
    //Directory with 1024 pdes (aka page tables)
    struct page_dir *page_dir = kzalloc(sizeof(struct page_dir));
    page_dir->page_tables = kzalloc(sizeof(uint32_t) * PAGE_TABLE_TOTAL); 

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
        page_dir->page_tables[i] = (uint32_t)pde | flags | PTE_FLAG_RW;

        //offset for next pde
        offset += PTE_PER_TABLE * PAGE_SIZE;
    }

    return page_dir;
}


/*
 * Sets pd as the system's page directory
 */
void page_set_dir(uint32_t *pd){
    page_load_dir(pd);
    current_pd = pd;
}
