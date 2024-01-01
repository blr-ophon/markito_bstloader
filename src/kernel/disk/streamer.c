#include "streamer.h"

struct disk_stream *diskstreamer_new(int disk_id){
    struct disk *disk = disk_get(disk_id);
    if(!disk){
        return 0;
    }

    struct disk_stream *streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;

    return streamer;
}

int diskstreamer_seek(struct disk_stream *stream, int pos){
    stream->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream *stream, void *out_buf, int size){
    int rv = 0;
    int sector = stream->pos / SECTOR_SIZE;
    int offset = stream->pos % SECTOR_SIZE;

    uint8_t buf[SECTOR_SIZE];
    rv |= disk_sector_read(stream->disk, sector, 1, buf);

    int bytes_read = size+offset > SECTOR_SIZE? SECTOR_SIZE - offset: size;
    
    uint8_t *cast_out_buf = (uint8_t*) out_buf;
    for(int i = 0; i < bytes_read; i++){
        *(cast_out_buf++) = buf[offset +i];
    }
    out_buf = cast_out_buf;

    stream->pos += bytes_read;

    if(size > SECTOR_SIZE){
        rv = diskstreamer_read(stream, out_buf, size - bytes_read);
    }

    return rv;
}

void diskstreamer_close(struct disk_stream *stream){
    kfree(stream);
}

