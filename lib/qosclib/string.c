#include "string.h"
#include "math.h"

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

size_t n_strnlen(const char *s, size_t maxlen){
    size_t rv;
    for(rv = 0; s[rv] != '\0' && rv < maxlen; rv++){
    }
    return rv;
}

int n_atoi(const char *s){
    int rv = 0;

    int sign_offset = 0;
    if(s[0] == '-'){
        sign_offset = 1;
    }

    //10 is the maximum number of digits a 32bit signed number
    //can have
    size_t len = n_strnlen(&s[sign_offset], 10);
    if(len == 0) return 0;

    for(int k = len -1 + sign_offset; k >= 0 + sign_offset; k--){
        rv += n_pow(10, len-k-1+sign_offset) * (s[k] - '0');
    }

    if(sign_offset) rv *= -1;

    return rv;
}
