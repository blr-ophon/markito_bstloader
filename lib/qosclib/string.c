#include "string.h"

void *n_memset(void *s, int c, size_t n){
    char *cPtr = (char*) s;     //casted Ptr
    for(int i = 0; i < n; i++){
        cPtr[i] = (char) c;
    }
    return s;
}

void *n_memcpy(void *dst, void *src, size_t n){
    //casted pointers
    char *cDst = (char*) dst;
    char *cSrc= (char*) src;

    for(int i = 0; i < n; i++){ 
        cDst[i] = cSrc[i];
    }

    return dst;
}

size_t n_strlen(const char *s){
    size_t rv;
    for(rv = 0; s[rv] != '\0'; rv++){
    }
    return rv;
}


