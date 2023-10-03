#include "kernel.h"

//TODO: heap_create is causing panic
//TODO: check for memory leaks

extern void int_test();

void screen_startup(void){
    vgam3_clear(MAIN_M3_COLOR);
    /*char markito[] = "        ####            \n"
                        "      ##########        \n"
                        "  ###################   \n"
                        "  ####################  \n"
                        " #####################  \n"
                        "##########  ##########  \n"
                        "#######       ########  \n"
                        "####             #####  \n"
                        " ##              #####  \n"
                        "  #               ####  \n"
                        "           ###########  \n"
                        "  #################    #\n"
                        "  #####  ##########  # #\n"
                        "  #####  #########    # \n"
                        "   ## #               # \n"
                        "     #                  \n"
                        "  #                     \n"
                        "  #                     \n"
                        "                #       \n"
                        "    # #####  ##      #  \n"
                        "      ##    ##          \n"
                        "      ######       #    \n"
                        "        ###             \n" 
                        "       #                \n"
                        "         #####          \n";
    vgam3_write(markito, 40, 0, MAIN_M3_COLOR);*/
    char title1[] = "=========================\n"
                    "  CursedOS by BLR-OPHON\n"
                    "=========================\n";
    vgam3_print(title1, MAIN_M3_COLOR);
}

static struct page_dir *Page_dir_32;

void kernel_main(void){
    screen_startup();

    //Initialize heap
    kheap_init();

    //Initialize Interrupt Descriptor Table
    idt_init();



    //Create a page directory for kernel
    Page_dir_32 = page_dir_init(PDE_FLAG_RW | PDE_FLAG_P | PDE_FLAG_US,
                                PTE_FLAG_RW | PTE_FLAG_P | PTE_FLAG_US);

    //Set Page_dir_32 as the page directory
    page_set_dir(Page_dir_32->page_tables);

    char *ptr = kzalloc(4096);
    page_set(Page_dir_32->page_tables, (void*) 0x1000, 
            (uint32_t)ptr | PTE_FLAG_RW | PTE_FLAG_P | PTE_FLAG_US);

    //Enable paging
    page_enable();

    /*
     * PAGING test
     */
    //write in virtual address 0x1000
    char *ptr2 = (char*) 0x1000;
    char *ptest_string = "> Paging OK\n";
    n_memcpy(ptr2, ptest_string, n_strlen(ptest_string));
    //read from physical address to see if has been modified
    vgam3_print(ptr, MAIN_M3_COLOR);




    struct path_root *path __attribute__((unused)) = pparser_parsePath("11:/foo/bar/");
}

