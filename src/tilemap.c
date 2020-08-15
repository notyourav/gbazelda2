#include "global.h"
#include "tilemap.h"
#include "empty_4bpp.h"
#include "empty_gbapal.h"

void LoadTiles() {
    REG_BG0CNT |= BG_MAP_BASE(8); // "Screen base block, 2kb * 8"
    REG_BG0CNT |= 1;
    //*(u16 *)MAP_BASE_ADR(8) = 
    dmaCopy(empty_4bpp, (u16*)VRAM, 32);
    dmaCopy(empty_gbapal, BG_PALETTE, 32);
}