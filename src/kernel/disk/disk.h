#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include "io_routines/io_r.h"
#include "string.h"
#include "config.h"
#include "status.h"

/*
 * Data is read and written in sectors. Typically 521 bytes
 * LBA: Logical block address. LBA 0 = first sector and so on.
 * LBA is sent through ports 0x1f2 to 0x1f6. The higher nibble
 * of 0x1f6 and 0x1f7 are for command/status
 */

typedef enum{
    DISK_TYPE_REAL = 0,
}DISK_TYPE;

struct disk{
    DISK_TYPE type;
    int sector_size;
};



/*
 * Initializes primary disk
 * TODO: Multiple disks not implemented yet
 */
void disk_search_and_init(void);

/*
 * Returns pointer to disk in index i
 * TODO: Multiple disks not implemented yet
 */
struct disk* disk_get(int index);

/*
 * Reads n sectors from specified disk and stores data in buf
 * TODO: Multiple disks not implemented yet
 */
int disk_sector_read(struct disk* idisk, uint32_t lba, uint8_t n, void *buf);


#endif
