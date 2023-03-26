#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define INTERRUPTS_TOTAL 512

#define HEAP_SIZE           104857600 //100MiB
#define HEAP_BLOCK_SIZE     4096
#define HET_TOTAL           (HEAP_SIZE/HEAP_BLOCK_SIZE)
#define HEAP_TABLE_ADDR     0x00007e00  //conventional memory block in real mode address space(<1 MiB)
#define HEAP_ADDR           0x01000000  //free for use RAM in extended memory (>1 MiB)

#endif
