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

void kernel_main(void){
    idt_init();
    kheap_init();
    screen_startup();
}

