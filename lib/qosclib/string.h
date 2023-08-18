#ifndef MEM_H
#define MEM_H

#include <stddef.h>

void *n_memset(void *s, int c, size_t n);
void *n_memcpy(void *dst, void *src, size_t n);
size_t n_strlen(const char *s);
size_t n_strnlen(const char *s, size_t maxlen);
int n_atoi(const char *s);

#endif
