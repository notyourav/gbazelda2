#include "global.h"
#include "oam.h"

OBJATTR OAMBuffer[128];
static u32 objectCount = 0;

// each tile is a word, 8x8 pixels
static u32 tileCount = 0;

u32 PaletteBuffer[16];
static u32 paletteCount = 0;

//---------------------------------------------------------------------------------
// Load 24x32 sized sprite. Stored as twelve 8x8 tiles in VRAM & OAM.
//
// To load a sprite with an existing palette/gfx, pass NULL for respective arg,
// and set set entity's respective index manually.
//---------------------------------------------------------------------------------
void LoadSprite(const u8* gfx, const u8* palette, Entity* ent, u32 shape)  {
    u32 objNum = 0;

    switch (shape) {
        case SIZE_16x16:
            objNum = 1;
            break;
        case SIZE_24x32:
            objNum = 12;
            break;
    }

    if (gfx != NULL) {
        ent->vramIndex = tileCount;
        switch (shape) {
            case SIZE_16x16:
                dmaCopy(gfx, &MEM_TILE[4][tileCount >> 1], 32 * 4);
                tileCount += 4;
                break;
            case SIZE_24x32:
                dmaCopy(gfx, &MEM_TILE[4][tileCount >> 1], 32 * 12);
                tileCount += 12;
                break;
        }
    }

    if (palette != NULL) {
        dmaCopy(palette, &PaletteBuffer[paletteCount], 32);
        ent->paletteIndex = paletteCount;
        paletteCount++;
    }

    if (ent != NULL) {
        ent->oamIndex = objectCount;
        ent->shape = shape;
    }

    objectCount += objNum;
}

void Update16x16(OBJATTR* base, Entity* ent) {
    int pOffX = 0;
    int pOffY = 0;

    if (ent->parent != NULL) {
        pOffX = ent->parent->pos.x;
        pOffY = ent->parent->pos.x;
    }
    base[0].attr0 = (ent->oamAttr0 << 8) + (u8)(ent->pos.y - pOffY + camera.y + (SCREEN_H / 2));
    base[0].attr1 = (ent->oamAttr1 << 8) + (u8)(ent->pos.x + pOffX - camera.x + (SCREEN_W / 2));
    base[0].attr2 = ent->vramIndex;
}

//---------------------------------------------------------------------------------
// Updates every tile for a 24x32 sized entity.
//---------------------------------------------------------------------------------
void Update24x32(OBJATTR* base, Entity* ent) {
    int i = 0;
    int j = 0;
    int c = 0;
    int pOffX = 0;
    int pOffY = 0;

    if (ent->parent != NULL) {
        pOffX = ent->parent->pos.x;
        pOffY = ent->parent->pos.x;
    }
    for (i = 0; i < 4; i++) {  
        for (j = 0; j < 3; j++) {
            base[c].attr0 = (ent->oamAttr0 << 8) + (u8)(i * 8 - ent->pos.y - pOffY + camera.y + (SCREEN_H / 2));
            base[c].attr1 = (ent->oamAttr1 << 8) + (u8)(j * 8 + ent->pos.x + pOffX - camera.x + (SCREEN_W / 2));
            base[c].attr2 = (ent->vramIndex + c);
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
        case SIZE_16x16:
            Update16x16(base, ent);
            break;
        case SIZE_24x32:
            Update24x32(base, ent);
            break;
    }
}

void CopyPaletteBuffer() {
    dmaCopy(PaletteBuffer, SPRITE_PALETTE, sizeof(PaletteBuffer));
}

void CopyOAMBuffer() {
    dmaCopy(OAMBuffer, (u16*)OAM, sizeof(OAMBuffer));
}