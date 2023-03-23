#include "io_r.h"

#define UNUSED_PORT 0x80

void ior_wait(void){
    //used to compensate PIC low speed, by
    //waiting between command words writing
    ior_outb(UNUSED_PORT, 0);
}
