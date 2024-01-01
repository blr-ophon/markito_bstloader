#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"
#include "memory/kheap.h"

struct disk_stream{
    int pos;
    struct disk *disk;
};


struct disk_stream *diskstreamer_new(int disk_id);
int diskstreamer_read(struct disk_stream *stream, void *out_buf, int size);
void diskstreamer_close(struct disk_stream *stream);

#endif
