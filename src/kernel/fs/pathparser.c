#include "pathparser.h"

#define MAX_PATH_SIZE       128        //TODO: move definition
#define MAX_FILENAME_SIZE   128 

//TODO: validate path name
//TODO: free path name

/*
 * Path format is:  x:/foo/bar/...
 * where x is the disk/partition 
 */
static int pparser_ValidatePath(const char *pathstr){
    //TODO: check other cases like empty pathname "0:/foo//bar"
    //more than 1 use of ':', use of whitespace and control words etc
    //should probably chech word by word while parsing the names
    int rv = 0;
    int len = n_strnlen(pathstr, MAX_PATH_SIZE);
    if(len < 3){ 
        rv = -EBADPATH;
        goto out;
    }
    if(!n_isdigit(pathstr[0])){ 
        rv = -EBADPATH;
        goto out;
    }
    if(n_memcmp((void*)&pathstr[1], ":/", 2)){
        rv = -EBADPATH;
        goto out;
    }

    for(int i = 0; i < len; i ++){
        if(!n_isascii(pathstr[i])){
            rv = -EBADPATH;
            goto out;
        }
    }

out:
    return rv;
}


/*
 * Parse path string
 */
struct path_root *pparser_parsePath(const char *pathstr){
    //TODO: check for invalid returns
    if(pparser_ValidatePath < 0){ 
        return NULL;
    }

    size_t plen = n_strnlen(pathstr, MAX_PATH_SIZE);
    char *pathstr_dup = kzalloc(plen+1);
    n_memcpy(pathstr_dup, (void*)pathstr, plen);

    int read_chars = 0;
    struct path_root *root = kzalloc(sizeof(struct path_root));

    //parse Drive 
    int disk_no = n_atoi(pathstr_dup);
    root->drive_no = disk_no;
    read_chars += 3;
    pathstr_dup += 3;

    //parse Tokens
    struct path_token *p = kzalloc(sizeof(struct path_token));
    struct path_token *first = p;
    while(read_chars < plen){
        p->next = kzalloc(sizeof(struct path_token));
        struct path_token *newToken = p->next;

        size_t token_len = (uint32_t)n_strchr(pathstr_dup, '/') - (uint32_t)pathstr_dup + 1;
        read_chars += token_len;

        n_memcpy((void*) newToken->token, pathstr_dup, token_len);
        pathstr_dup += token_len;

        p = newToken; 
    }

    root->first = first->next; 
    kfree(first);


    kfree(pathstr_dup);
    return root;
}
