#include "paging/paging.h"
#include "status.h"

//TODO: Page table dump and page dump functions for debug

//global variable holding page directory address
uint32_t *current_pd = NULL;


/*
 * Creates a page directory with all page tables set with 'pde_flags' and RW flags
 * Returns directory.
 */
struct page_dir *page_dir_init(uint8_t pde_flags, uint8_t pte_flags){
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
            pde[j] = (offset + (j * PAGE_SIZE)) | pte_flags;
        }

        //set pde in page directory
        page_dir->page_tables[i] = (uint32_t)pde | pde_flags | PDE_FLAG_RW;

        //offset for next pde
        offset += PTE_PER_TABLE * PAGE_SIZE;
    }

    return page_dir;
}

/*
 * Sets pd as the system's page directory
 */
void page_set_dir(uint32_t *pd){
    //moves pd to cr3 register
    page_load_dir(pd);  
    //sets global variable to pd
    current_pd = pd;    
}


bool page_is_aligned(void *addr){
    return ((uint32_t) addr % PAGE_SIZE) == 0;
}

/*
 * Sets variables with the index of the page table in the directory and the index of the page in
 * the page table
 */
int page_get_indexes(void *virt_addr, uint32_t *table_idx, uint32_t *page_idx){
    int res = 0;
    if(!page_is_aligned(virt_addr)){
        res = -EINVARG;
        goto out;
    }
     
    //each directory entry (page table) has 1024*4096 bytes
    *table_idx = ((uint32_t)virt_addr) / (PTE_PER_TABLE * PAGE_SIZE);

    //each page has 4096 bytes, each table has 1024 pages
    *page_idx = ((uint32_t)virt_addr / PAGE_SIZE) % (PTE_PER_TABLE);

out:
    return res;
}

/*
 * Set page in specified virtual address to val. virt_addr must be 
 * aligned to 4096
 */
int page_set(uint32_t *pd, void *virt_addr, uint32_t val){
    int res = 0;
    if(!page_is_aligned(virt_addr)){
        res = -EINVARG;
        goto out;
    }

    //get indexes
    uint32_t table_idx = 0;
    uint32_t page_idx = 0;
    res |= page_get_indexes(virt_addr, &table_idx, &page_idx);
    if(res < 0) goto out;

    //get page table address
    uint32_t table = pd[table_idx];
    uint32_t *table_addr = (uint32_t*)(table & 0xfffff000);  //ignore last 12 bits (flags)
                                                             
    //set page entry in the table to val
    table_addr[page_idx] = val;

out:
    return res;
}
