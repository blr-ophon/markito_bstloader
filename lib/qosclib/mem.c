#include "mem.h"

void *n_memset(void *s, int c, size_t n){
    char *cPtr = (char*) s;
    for(int i = 0; i < n; i++){
        cPtr[i] = (char) c;
    }
    return s;
}
