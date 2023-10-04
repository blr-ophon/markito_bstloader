#ifndef DSTREAMER_H
#define DSTREAMER_H

#include "disk.h"

struct diskStream{
    int pos;
    struct disk* disk;
};



struct diskStream *DStreamer_create(int disk_id);

int DStreamer_seek(struct diskStream *stream, int pos);

int DStreamer_read(struct diskStream *stream, void *out, int total);

void DStreamer_free(struct diskStream *stream);


#endif
