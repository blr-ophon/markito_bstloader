#include "kernel.h"

uint16_t *vga_m3_buffer = 0;
int vga_text_x = 0;
int vga_text_y = 0;

void screen_init(uint8_t color){
    uint16_t full_char = ((uint16_t)color << 8) | ' ';
    vga_m3_buffer = (uint16_t*)0xb8000;
    for(int i = 0; i < VGA_MODE3_H; i++){
        for(int j = 0; j < VGA_MODE3_W; j++){
            vga_m3_buffer[VGA_MODE3_W*i + j] = (uint16_t) full_char;
        }
    }
}

void vgam3_write(char *c, int x, int y, uint8_t color){
    int c_x = x;
    int c_y = y;
    for(int i = 0; c[i] != 0; i++){
        uint16_t full_char = ((uint16_t)color << 8) | c[i];
        if(c_x >= VGA_MODE3_W || c[i] == '\n') {c_x = x; c_y++;}
        if(c[i] == '\n') {continue;}
        vga_m3_buffer[VGA_MODE3_W*c_y + c_x] = full_char;
        c_x++;
    }
}

void vgam3_print(char *c, uint8_t color){
    for(int i = 0; c[i] != 0; i++){
        uint16_t full_char = ((uint16_t)color << 8) | c[i];
        if(c[i] == '\n') {vga_text_x = 0; vga_text_y++; continue;}
        vga_m3_buffer[VGA_MODE3_W*vga_text_y + vga_text_x] = full_char;
        vga_text_x++;
    }
}

void kernel_main(void){
    screen_init(MAIN_M3_COLOR);
    char title1[] = "CursedOS by BLR-OPHON\n"
                    "Version 1.0\n";
    char markito[] = "Golden Markito Seal seal\n";
    char markito_seal[] = "        ####            \n"
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
    vgam3_print(title1, MAIN_M3_COLOR);
    vgam3_write(markito_seal, 40, 0, 0x1e);
    vgam3_write(markito, 0, 20, 0x1e);

}

