#include "paging/paging.h"

struct page_dir *page_dir_init(uint8_t flags){
    //Set Page directory 
    
    int offset = 0;
    //Directory with 1024 pdes (aka page tables)
    uint32_t *PDEs = kzalloc(sizeof(uint32_t) * PAGING_PDE_TOTAL); 
    for(int i = 0; i < PAGING_PDE_TOTAL; i++){

        //pde with 1024 ptes (page table entries)
        uint32_t *pde = kzalloc(sizeof(uint32_t) * PAGING_PTE_PER_PDE);
        for(int j = 0; j < PAGING_PTE_PER_PDE; j++){
            //set pte in page table
            //address on last 20 bits, flags on first 12 (8 for 4Kib pages) bits
            pde[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
        }

        //set pde in page directory
        PDEs[i] = (uint32_t)pde | flags | PAGE_FLAG_RW;

        //offset for next pde
        offset += PAGING_PTE_PER_PDE * PAGING_PAGE_SIZE;
    }

    struct page_dir *page_dir = kzalloc(sizeof(struct page_dir));
    page_dir->PDEs = PDEs;
    
    return page_dir;
}
