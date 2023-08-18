#ifndef PATHPARSER_H
#define PATHPARSER_H

#include "status.h"
#include "ctype.h"
#include "string.h"
#include "memory/kheap.h"

struct path_root{
    int drive_no;
    struct path_token* first;
};

struct path_token{
    const char token[256];
    //TODO: use dynamic memory allocation when you implement a better heap
    struct path_token *next;
};


struct path_root *pparser_parsePath(const char *pathstr);

#endif
