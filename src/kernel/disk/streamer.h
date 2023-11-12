#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"

struct disk_stream{
    int pos;
    struct disk *disk;
};

#endif
