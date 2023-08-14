#include "kernel.h"

//TODO: heap_create is causing panic

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
    Page_dir_32 = page_dir_init(PTE_FLAG_RW | PTE_FLAG_P | PTE_FLAG_US);

    //Set Page_dir_32 as the page directory
    page_set_dir(Page_dir_32->page_tables);

    //Enable paging
    page_enable();
}

