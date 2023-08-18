#ifndef PATHPARSER_H
#define PATHPARSER_H

struct path_root{
    int drive_no;
    struct path_part* first;
};

struct path_token{
    const char *token;
    struct path_token *next;
};

#endif
