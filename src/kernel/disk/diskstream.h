#ifndef STREAMER_H
#define STREAMER_H

#include "disk.h"
#include "memory/kheap.h"

struct disk_stream{
    int pos;
    struct disk *disk;
};


struct disk_stream *diskstreamer_create(int disk_id);
void diskstreamer_close(struct disk_stream *stream);
int diskstreamer_seek(struct disk_stream *stream, int pos);
int diskstreamer_read(struct disk_stream *stream, void *out_buf, int size);

#endif
