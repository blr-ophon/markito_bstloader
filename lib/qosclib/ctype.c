#include "ctype.h"

int n_isdigit(int c){
    return c >= '0' && c <= '9';
}

int n_isalpha(int c){
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int n_islower(int c){
    return (c >= 'a' && c <= 'z');
}

int n_isupper(int c){
    return (c >= 'A' && c <= 'Z');
}

int n_isalnum(int c){
    return n_isalpha(c) || n_isdigit(c);
}

int n_isascii(int c){
    return ((unsigned) c <= 0x7f);
}

int n_iscntrl(int c){
    return ((unsigned) c > 0x1f && (unsigned) c != 0x7f);
}
