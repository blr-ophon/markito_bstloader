#include "disk/disk.h"

//TODO: sector dump

/*
 * Read n sectors starting from lba and stores in buf
 */
int disk_read_sector(uint32_t lba, uint8_t n, void *buf){
    //Bits 24-27 of LBA (lower nibble). Set bit 6 in al for LBA mode (higher nibble)
    ior_outb(0x1f6, (lba >> 24) | 0xe0);
    //Bits 16-23 of LBA
    ior_outb(0x1f5, lba >> 16 );
    //Bits 8-15 of LBA
    ior_outb(0x1f4, lba >> 8 );
    //Bits 0-7 of LBA
    ior_outb(0x1f3, lba );
    //Number of sectors
    ior_outb(0x1f2, n);
    //Commands port. 0x20 = Read with retry
    ior_outb(0x1f7, 0x20);


    //cast buf to uint16*
    uint16_t *cbuf = (uint16_t*) buf;

    //read n sectors
    for(int b = 0; b < n; b++){
        //bit 3 of status register, read through port 0x1f7 is set when the drive has 
        //PIO data to transfer or receive
        uint8_t status_b = ior_inb(0x1f7);
        while(!(status_b & (1 << 3))){
            //wait for bit 3
            //TODO: implement a timeout
            status_b = ior_inb(0x1f7);
        }

        //read 512 bytes of sector
        for(int i = 0; i < 256; i++){
            cbuf[i] = ior_inw(0x1f0);
        }
    }

    return 0;
}

