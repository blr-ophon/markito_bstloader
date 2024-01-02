#include "streamer.h"

//TODO: Test.

/*
 * Create new stream of a disk
 */
struct disk_stream *diskstreamer_create(int disk_id){
    struct disk *disk = disk_get(disk_id);
    if(!disk){
        return 0;
    }

    struct disk_stream *streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;

    return streamer;
}

/*
 * Set the position of the "cursor" in the stream
 */
int diskstreamer_seek(struct disk_stream *stream, int pos){
    stream->pos = pos;
    return 0;
}

/*
 * Read 'size' bytes from the stream to an output buffer
 */
int diskstreamer_read(struct disk_stream *stream, void *out_buf, int size){
    int rv = 0;

    uint8_t buf[SECTOR_SIZE];

    int remn_bytes = size;  //Remaining bytes
    
    while(remn_bytes > 0){
        int sector = stream->pos / SECTOR_SIZE;
        int offset = stream->pos % SECTOR_SIZE;

        //Read entire sector from disk to buf
        
        rv |= disk_sector_read(stream->disk, sector, 1, buf);
        if(rv < 0) goto out;

        //Read what is requested from buf to out_buf
        
        int bytes_read = remn_bytes+offset > SECTOR_SIZE? SECTOR_SIZE - offset: remn_bytes;
        
        uint8_t *cast_out_buf = (uint8_t*) out_buf;
        for(int i = 0; i < bytes_read; i++){
            *(cast_out_buf++) = buf[offset +i];
        }
        out_buf = cast_out_buf;


        stream->pos += bytes_read;
        remn_bytes -= bytes_read;
    }

out:
    return rv;
}

void diskstreamer_close(struct disk_stream *stream){
    kfree(stream);
}

