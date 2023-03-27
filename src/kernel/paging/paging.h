#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "memory/kheap.h"

//PDE: Page Directory Entry = Page Table
//PTE: Page Table Entry

//Page Directory contains 1024 PDEs, which contain 1024 PTEs each.
//Each table entry is responsible for 4KiB of memory, so the total
//for IA-32 is 1024*1024*4KiB = 4GiB of RAM

//Because all pages are responsible for 4KiB of memory and are aligned,
//all of them have addresses ending with 0x000, that's why they are 
//represented with only 20 bits (32 - 12).

#define PAGING_PDE_TOTAL    1024
#define PAGING_PTE_PER_PDE  1024
#define PAGING_PAGE_SIZE    4096

struct page_dir{
    uint32_t *PDEs;
};

typedef enum {
    PAGE_FLAG_P                 = (1 << 0),     //Present
    PAGE_FLAG_RW                = (1 << 1),     //R/W permission
    PAGE_FLAG_US                = (1 << 2),     //User/Supervisor
    PAGE_FLAG_PWT               = (1 << 3),     //Write-through caching or write-back
    PAGE_FLAG_PCD               = (1 << 4),     //Cache Disabled
    PAGE_FLAG_A                 = (1 << 5),     //Accessed
    PAGE_FLAG_PS                = (1 << 7),     //Page Size. Set for 4MiB page, clear for 4KiB
                                                
    //for 4MB pages
    PAGE_FLAG_D                 = (1 << 6),     //'Dirty'. Has been written to
    PAGE_FLAG_G                 = (1 << 8),     //Global
    PAGE_FLAG_PAT               = (1 << 12),    //Page Attribute Table. If set, PAT, PCD and
                                                //PWT indicate memory caching type
} PAGING_FLAGS;

#endif
