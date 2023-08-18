#include "pathparser.h"

#define MAX_PATH_SIZE       128        //TODO: move definition
#define MAX_FILENAME_SIZE   128 

//TODO: validate path name
//TODO: free path name

/*
 * path format is:  x:/foo/bar/...
 * where x is the disk/partition 
 */
static int pparser_ValidatePath(const char *pathstr){
    int rv = 1;
    int len = n_strnlen(pathstr, MAX_PATH_SIZE);
    if(len < 3){ 
        rv = 0;
        goto out;
    }
    if(!n_isdigit(pathstr[0])){ 
        rv = 0;
        goto out;
    }
    if(n_memcmp((void*)&pathstr[1], ":/", 2)){
        rv = 0;
        goto out;
    }

out:
    return rv;
}

/*
 * Reads drive, returns its value and advances pathstr to next token
 */
static int pparser_parseDrive(char **pathstr){
    if(!pparser_ValidatePath(*pathstr)){
        return -EBADPATH;
    }

    int rv = n_atoi(*pathstr);

    *pathstr += 3;
    return rv;
}

/*
 * parse path string
 */
struct path_root *pparser_parsePath(const char *pathstr){
    //TODO: check for invalid returns
    size_t plen = n_strnlen(pathstr, MAX_PATH_SIZE);
    char *pathstr_dup = kzalloc(plen+1);
    n_memcpy(pathstr_dup, (void*)pathstr, plen);

    int read_chars = 0;

    //parse Drive 
    struct path_root *root = kzalloc(sizeof(struct path_root));

    struct path_token *test __attribute__((unused));

    int rv = pparser_parseDrive(&pathstr_dup);
    if(rv >= 0){
        root->drive_no = rv;
        read_chars += 3;

        //parse Tokens
        struct path_token *p = kzalloc(sizeof(struct path_token));
        struct path_token *first = p;
        test = p;
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
    }


    kfree(pathstr_dup);
    return root;
}
