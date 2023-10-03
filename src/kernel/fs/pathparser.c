#include "pathparser.h"

#define MAX_PATH_SIZE       128        //TODO: move definition
#define MAX_FILENAME_SIZE   128 

/*
 * Validate that a path is in the format:  (num):/foo/bar/...
 * where (num) is the disk/partition 
 */
static int pparser_ValidatePath(const char *pathstr){
    int rv = 0;
    int len = n_strnlen(pathstr, MAX_PATH_SIZE);
    if(len < 3){ 
        rv = -EBADPATH;
        goto out;
    }

    //Filenames can have any ascii characters except control characters 
    for(int i = 0; i < len; i ++){
        if(!n_isascii(pathstr[i]) || n_iscntrl(pathstr[i])){
            rv = -EBADPATH;
            goto out;
        }
    }

    //empty token "(num):/foo//bar" 
    for(int i = 0; i < len-1; i++){
        if(pathstr[i] == '/' && pathstr[i+1] == '/'){
            return -EBADPATH;
        }
    }

    //start is not in the format: (num):/
    int read_digits = 0;
    for(int i = 0; n_isdigit(pathstr[i]); i++){
        read_digits++;
    }
    if(read_digits == 0 || pathstr[read_digits] != ':' || pathstr[read_digits+1] != '/'){
        return -EBADPATH;
    }

out:
    return rv;
}


void pparser_free(struct path_root *root){
    struct path_token *node = root->first;
    while(node){
        struct path_token *tmp = node->next;
        kfree(node);
        node = tmp;
    }
}


/*
 * Parse path string
 */
struct path_root *pparser_parsePath(const char *path, const char *cd_path){
    if(pparser_ValidatePath < 0){ 
        return NULL;
    }

    size_t plen = n_strnlen(path, MAX_PATH_SIZE);
    char *pathstr_dup = kzalloc(plen+1);
    n_memcpy(pathstr_dup, (void*)path, plen);

    int read_chars = 0;
    struct path_root *root = kzalloc(sizeof(struct path_root));


    //parse Drive 
    int disk_no = n_atoi(pathstr_dup);
    while(n_isdigit(*pathstr_dup)){
        read_chars++;
        pathstr_dup ++;
    }
    root->drive_no = disk_no;
    read_chars += 2;
    pathstr_dup += 2;


    //parse Tokens
    struct path_token *p = kzalloc(sizeof(struct path_token));
    struct path_token *first = p;

    while(read_chars < plen){
        p->next = kzalloc(sizeof(struct path_token));
        struct path_token *newToken = p->next;

        size_t token_len = (uint32_t)n_strchr(pathstr_dup, '/') - (uint32_t)pathstr_dup + 1;
        read_chars += token_len;

        n_memcpy((void*) newToken->token, pathstr_dup, token_len-1);
        pathstr_dup += token_len;

        p = newToken; 
    }

    root->first = first->next; 
    kfree(first);

    kfree(pathstr_dup);
    return root;
}
