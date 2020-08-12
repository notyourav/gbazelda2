#ifndef SPRITE_H
#define SPRITE_H

#include "global.h"
#include "main.h"
#include "entity.h"

typedef u32 Tile[16];
typedef Tile TileBlock[256];
typedef Tile Tile8x8[64];
#define MEM_TILE ((TileBlock*)0x6000000 )
#define MEM_TILE8x8 ((Tile8x8*)0x6000000)
#define MEM_OAM  ((OBJATTR *)0x07000000)
extern u16 bgPalBuffer[16];
extern u16 objPalBuffer[16];

extern OBJATTR OAMBuffer[128];

void LoadSprite(const u8* gfx, const u8* palette, Entity* ent, u32 shape);
void UpdateObjectAttributes(Entity* ent);
void CopyPaletteBuffer();
void CopyOAMBuffer();
#endif