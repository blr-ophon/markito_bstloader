#ifndef MEM_H
#define MEM_H

#include <stddef.h>

size_t n_strlen(const char *s);
size_t n_strnlen(const char *s, size_t maxlen);
char *n_strchr(char *s, int c);
void *n_memset(void *s, int c, size_t n);
void *n_memcpy(void *dst, void *src, size_t n);
int n_memcmp(const void *s1, const void *s2, size_t n);
int n_atoi(const char *s);

#endif
