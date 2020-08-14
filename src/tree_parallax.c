#include "global.h"
#include "forest_bg_pcx.h"
#include "tree_gbapal.h"
#include "fade.h"
#include "pcx.h"

//u8 palBuffer[256];

void Tree_Parallax() {
    DecodePCX(forest_bg_pcx, (u16*)VRAM, BG_PALETTE);
    //REG_BG0CNT |= 0b11;
    //dmaCopy(forest_bg_4bpp, (u16 *)VRAM, forest_bg_4bpp_size);
    //dmaCopy(forest_bg_4bpp, (u8 *)VRAM + forest_bg_4bpp_size, forest_bg_4bpp_size);
    //dmaCopy(forest_bg_4bpp, (u16 *)VRAM + forest_bg_4bpp_size, forest_bg_4bpp_size);
    for (int i = 0; i < 16; i++) {
        //dmaCopy(tree_gbapal, (u8 *)BG_PALETTE + i * tree_gbapal_size, tree_gbapal_size);
    }
}