#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include "io_routines/io_r.h"

/*
 * Data is read and written in sectors. Typically 521 bytes
 * LBA: Logical block address. LBA 0 = first sector and so on.
 *
 * Ports:
 * 0x1f6: Bits 24-27 of LBA (lower nibble). Commands (Higher nibble) 
 * 0x1f5: Bits 16-23 of LBA
 * 0x1f4: Bits 8-15 of LBA
 * 0x1f3: Bits 0-7 of LBA
 * 0x1f2: Number of sectors
 * 0x1f7: Command(W)/Status(R) port
 *        bit 3 of status register is set when the drive has...
 *        ... PIO data to transfer or receiver
 */


int disk_read_sector(uint32_t lba, uint8_t n, void *buf);



#endif
