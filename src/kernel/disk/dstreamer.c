#include "dstreamer.h"
#include "memory/kheap.h"

struct diskStream *DStreamer_create(int disk_id){
    struct disk *disk = disk_get(disk_id);
    if(!disk){
        return NULL; 
    }

    struct diskStream *streamer = kzalloc(sizeof(struct diskStream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}


int DStreamer_seek(struct diskStream *stream, int pos){
    stream->pos = pos;
    return 0;
}

int DStreamer_read(struct diskStream *stream, void *out, int total){
    //TODO: avoid recursion
    //TODO: change variable names
    //TODO: test

    //read entire sector to buf
    int lba = stream->pos / SECTOR_SIZE;
    char buf[SECTOR_SIZE];

    int rv = disk_sector_read(stream->disk, lba, 1, buf);
    if(rv < 0){
        goto out;
    }

    //read from buf to out
    int offset = stream->pos % SECTOR_SIZE;
    int readSize = total > SECTOR_SIZE? SECTOR_SIZE : total;
    for(int i = 0; i < readSize; i++){
        *((char*)out++) = buf[offset+i];
    }

    //read again if more than one sector must be read
    stream->pos += readSize;
    if(total > SECTOR_SIZE){
        rv = DStreamer_read(stream, out, total - SECTOR_SIZE);
    }

out:
    return rv;
}

void DStreamer_free(struct diskStream *stream){
    kfree(stream);
}
