#include "string.h"
#include "math.h"
#include "ctype.h"

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

/*
 * Returns address where c is found or the address of the null character
 */
char *n_strchr(char *s, int c){
    int i;
    for(i = 0; s[i] != '\0'; i++){
        if(s[i] == c){
            break;
        }
    }

    return &s[i];
}


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

int n_memcmp(const void *s1, const void *s2, size_t n){
    //cast pointers
    const char *cs1 = (char*) s1;
    const char *cs2 = (char*) s2;

    int rv = 0;
    for(int i = 0; i < n; i++){
        if(cs1[i] != cs2[i]){
            rv = cs1[i] > cs2[i] ? 1 : -1;
            break;
        }
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
    int len; 
    for(len = 0; n_isdigit(s[len]) && len < 10; len++){
    }
    if(len == 0) return 0;

    for(int k = len -1 + sign_offset; k >= 0 + sign_offset; k--){
        if(!n_isdigit(s[k])) break;
        rv += n_pow(10, len-k-1+sign_offset) * (s[k] - '0');
    }

    if(sign_offset) rv *= -1;

    return rv;
}

