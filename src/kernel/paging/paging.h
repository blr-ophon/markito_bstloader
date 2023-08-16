#ifndef PAGING_H
#define PAGING_H
/*
 * PDE: Page Directory Entry
 * PTE: Page Table Entry
 * 
 * Page Directory contains 1024 Page Tables, which contain 1024 PTEs of 32bits each.
 * Each table entry is responsible for 4KiB of memory, so the total
 * for IA-32 is 1024*1024*4KiB = 4GiB of RAM
 * 
 * Because all pages are responsible for 4KiB of memory and are aligned,
 * all of them have addresses ending with 0x000, that's why they are 
 * represented with only 20 bits (32 - 12).
 *
 *
 * To enable 32-bit paging, all that is needed is to load cr3
 * with the address of the page directory and to set the paging
 * (PG) and protection (PE) bits of cr0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "memory/kheap.h"

#define PAGE_TABLE_TOTAL    1024        //total number of page tables in the directory
#define PTE_PER_TABLE       1024        //number of entries in each page table
#define PAGE_SIZE           4096        //size of a single table

struct page_dir{
    /*
     * page_dir is an array of 1024 page tables,
     *
     * each page table is a 4096 aligned address that 
     * points to an array of 1024 pages plus 12 bits of flags
     *
     * a page is a 4096 aligned address plus 12 bits of flags
     */
    uint32_t *page_tables;
};


typedef enum {
    PDE_FLAG_P                 = (1 << 0),     //Present. Set if page is currently in physical memory
    PDE_FLAG_RW                = (1 << 1),     //R/W permission
    PDE_FLAG_US                = (1 << 2),     //User/Supervisor. Set if may be accessed by all
    PDE_FLAG_PWT               = (1 << 3),     //Memory caching type. Write-through caching or write-back
    PDE_FLAG_PCD               = (1 << 4),     //Cache Disable
    PDE_FLAG_A                 = (1 << 5),     //Accessed
    PDE_FLAG_PS                = (1 << 7),     //Page Size. Set for 4MiB page, clear for 4KiB
} PAGE_TABLE_FLAGS;


typedef enum {
    PTE_FLAG_P                 = (1 << 0),     //Present. Set if page is currently in physical memory
    PTE_FLAG_RW                = (1 << 1),     //R/W permission
    PTE_FLAG_US                = (1 << 2),     //User/Supervisor. Set if may be accessed by all
    PTE_FLAG_PWT               = (1 << 3),     //Memory caching type. Write-through caching or write-back
    PTE_FLAG_PCD               = (1 << 4),     //Cache Disable
    PTE_FLAG_A                 = (1 << 5),     //Accessed
    PTE_FLAG_D                 = (1 << 6),     //'Dirty'. Has been written to
    PTE_FLAG_G                 = (1 << 8),     //Global
    PTE_FLAG_PAT               = (1 << 7),    //Page Attribute Table. If set, PAT, PCD and
} PAGE_FLAGS;



/*
 * Creates a page directory with all pages set with 'flags'.
 * Returns directory.
 */
struct page_dir *page_dir_init(uint8_t pde_flags, uint8_t pte_flags);

void page_set_dir(uint32_t *pd);
int page_set(uint32_t *pd, void *virt_addr, uint32_t val);


/*
 * ASM functions
 */

void page_enable();
void page_load_dir(uint32_t *pd);

#endif
