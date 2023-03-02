#include "kernel.h"

void screen_startup(void){
    vgam3_clear(MAIN_M3_COLOR);
    char markito[] = "        ####            \n"
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
    char title1[] = "CursedOS by BLR-OPHON\n"
                    "Version 1.0\n";
    vgam3_write(markito, 40, 0, MAIN_M3_COLOR);
    vgam3_print(title1, MAIN_M3_COLOR);
}

void kernel_main(void){
    screen_startup();
}

