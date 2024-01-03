#include "disk/disk.h"
//TODO: sector hexdump
//TODO: implement multiple disks
#include "vgam3.h"


int ata_read_lba(uint32_t lba, uint8_t n, void *buf);

struct disk disk;   //global variable with main disk

/*
 * Initializes primary disk
 */
void disk_search_and_init(void){
    n_memset(&disk, 0, sizeof(disk));
    disk.type = DISK_TYPE_REAL;
    disk.sector_size = SECTOR_SIZE;
}

/*
 * Returns pointer to disk in index i
 * TODO: Multiple disks not implemented yet
 */
struct disk* disk_get(int index){
    if(index != 0){
        return 0;
    }

    return &disk;
}



/*
 * Reads n sectors from specified disk and stores data in buf
 * TODO: Multiple disks not implemented yet
 */
int disk_sector_read(struct disk* idisk, uint32_t lba, uint8_t n, void *buf){
    if(idisk != &disk){
        return -EIO;
    }

    return ata_read_lba(lba, n, buf);
}



/*
 * Read n sectors starting from lba and stores in buf
 */
int ata_read_lba(uint32_t lba, uint8_t n, void *buf){
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



