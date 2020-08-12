#include "global.h"
#include "oam.h"

OBJATTR OAMBuffer[128];
static u32 objectCount = 0;

u32 PaletteBuffer[16];
static u32 paletteCount = 0;

//---------------------------------------------------------------------------------
// Load 16x16 sized sprite. Stored as a single 16x16 tile in VRAM & OAM.
//
// To load a sprite with an existing palette, pass NULL for palette data,
// and set set entity's palette index manually.
//---------------------------------------------------------------------------------
void Load16x16(OBJATTR* gfx, const u8* palette, Entity* ent) {
}

//---------------------------------------------------------------------------------
// Load 24x32 sized sprite. Stored as twelve 8x8 tiles in VRAM & OAM.
//
// To load a sprite with an existing palette, pass NULL for palette data,
// and set set entity's palette index manually.
//---------------------------------------------------------------------------------
void Load24x32(OBJATTR* gfx, const u8* palette, Entity* ent)  {
    int i;
    for (i = 0; i < 12; i++) {
        OAMBuffer[objectCount + i] = gfx[i];
    }

    if (palette != NULL) {
        dmaCopy(palette, &PaletteBuffer[paletteCount], 32);
        //PaletteBuffer[paletteCount] = *(u32 *)palette;
        ent->paletteIndex = paletteCount;
        paletteCount++;
    }

    if (ent != NULL) {
        ent->oamIndex = objectCount;
        ent->shape = SIZE_24x32;
    }
    objectCount += 12;
}

//---------------------------------------------------------------------------------
// Updates every tile for a 24x32 sized entity.
//---------------------------------------------------------------------------------
void Update24x32(OBJATTR* base, Entity* ent) {
    int i, j, c = 0;
    for (i = 0; i < 4; i++) {  
        for (j = 0; j < 3; j++) {
            base[c].attr0 = ent->oamAttr0 + (u8)(i * 8 - ent->pos.y + camera.y + 90);
            base[c].attr1 = ent->oamAttr1 + (u8)(j * 8 + ent->pos.x - camera.x + 120);
            base[c].attr2 = ent->oamIndex + c;
            c++;
        }
    }
}

//---------------------------------------------------------------------------------
// Updates any entity provided, as long as it has its object shape and OAM index
// assigned.
//---------------------------------------------------------------------------------
void UpdateObjectAttributes(Entity* ent) {
    OBJATTR* base = &OAMBuffer[ent->oamIndex];

    switch (ent->shape) {
        case NONE:
            break;
        case SIZE_24x32:
            Update24x32(base, ent);
    }
}

void CopyPaletteBuffer() {
    dmaCopy(PaletteBuffer, SPRITE_PALETTE, sizeof(PaletteBuffer));
}

void CopyOAMBuffer() {
    dmaCopy(OAMBuffer, (u16*)OAM, sizeof(OAMBuffer));
}