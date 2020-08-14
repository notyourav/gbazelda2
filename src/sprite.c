#include "global.h"
#include "sprite.h"
#include "tonc_nocash.h"
#include <stdio.h>

EWRAM_DATA OBJATTR OAMBuffer[128];
static u32 objectCount = 0;

// each tile is a word, 8x8 pixels
u32 tileCount = 0;

EWRAM_DATA u8 PaletteBuffer[64];
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
            ent->attr1.f.objSize = 1;
            objNum = 1;
            break;
        case SIZE_16x32:
            ent->attr1.f.objSize = 2;
            ent->attr0.f.shape = 2;
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
                dmaCopy(gfx, SPR_VRAM(tileCount), 32 * 4);
                ent->center.x = 8;
                ent->center.y = 8;
                tileCount += 4;
                break;
            case SIZE_16x32:
                dmaCopy(gfx, SPR_VRAM(tileCount), 32 * 8);
                ent->center.x = 8;
                ent->center.y = 16;
                tileCount += 8;
                break;
            case SIZE_24x32:
                dmaCopy(gfx, SPR_VRAM(tileCount), 32 * 12);
                ent->center.x = 11;
                ent->center.y = 16;
                tileCount += 12;
                break;
        }
    }

    if (palette != NULL) {
        dmaCopy(palette, &PaletteBuffer[paletteCount * 32], 32);
        ent->paletteIndex = paletteCount++;
    }

    ent->oamIndex = objectCount;
    ent->shape = shape;

    objectCount += objNum;
}


//---------------------------------------------------------------------------------
// Handle sprite flipping, offsetting, and OAM attributes.
//---------------------------------------------------------------------------------
void UpdateSprite(OBJATTR* base, Entity* ent, const Keyframe* curFrame) {
    u32 oamWidth = 0;
    u32 oamHeight = 0;
    u32 gfxSize = 1;

    int offX = 0;
    int offY = 0;

    //---------------------------------------------------------------------------------
    // oamWidth: width in oam objects
    // oamHeight: height in oam objects
    // gfxSize: number of VRAM tiles * size of a (4bpp) VRAM tile
    //---------------------------------------------------------------------------------
    switch (ent->shape) {
        case SIZE_16x16:
            oamWidth = 1;
            oamHeight = 1;
            gfxSize = 4 * 32;
            break;
        case SIZE_16x32:
            oamWidth = 1;
            oamHeight = 1;
            gfxSize = 8 * 32;
            break;
        case SIZE_24x32:
            oamWidth = 3;
            oamHeight = 4;
            gfxSize = 12 * 32;
            break;
    }

    //---------------------------------------------------------------------------------
    // Handle whether or not the sprite should be flipepd and the offset accordingly
    // Q: should the entity be flipped this frame? 
    // A: Only if the keyframe specifies so or the parent is flipped, but not BOTH.
    //
    // Additionally, if this entity has a parent and this entity is flipped,
    // then we also mirror the offset given by the frame.
    //---------------------------------------------------------------------------------
    if (curFrame != NULL) { 
        if (ent->parent != NULL) {
            ent->attr1.f.flipX = curFrame->flipX ^ ent->parent->attr1.f.flipX;

            if (ent->attr1.f.flipX) {
                offX = -curFrame->xOff;
            }
            else {
                offX = curFrame->xOff;
            }

            if (ent->attr1.f.flipY) {
                offY = -curFrame->yOff;
            }
            else {
                offY = curFrame->yOff;
            }
        }
        else {
            offX = curFrame->xOff;
            offY = curFrame->yOff;
        }
        dmaCopy(ent->animation->gfx + (curFrame->gfxOffset * gfxSize), SPR_VRAM(ent->vramIndex), gfxSize);
    }

    //---------------------------------------------------------------------------------
    // Apply parent position
    //---------------------------------------------------------------------------------
    if (ent->parent != NULL) {
        offX += ent->parent->pos.x;
        offY += ent->parent->pos.y;
    }


    //---------------------------------------------------------------------------------
    // Update OAM tiles
    //---------------------------------------------------------------------------------
    
    u32 i = 0;
    u32 j = 0;
    u32 count = 0;

        for (i = 0; i < oamHeight; i++) {
            // TODO: handle vertical flipping for metasprites
            if ((oamWidth > 1) && (((base[count].attr1 >> 8) & 0x10) != ent->attr1.f.flipX)) {
                // TODO: make sure this doesnt just work with 3 wide metasprites
                for (j = oamWidth; j > 0; j--) {
                    //              attr0 = upper 8 bytes                                when all of these are zero, sprite should be centered on screen
                    //                      \                   I forget why this is x2          _____/|\________________________
                    //                       \                           |                      /      |           |             |
                    base[count].attr0 = (ent->attr0.raw << 8) + (u8)(i * 2 * oamHeight - ent->pos.y - offY - ent->center.y + camera.y + (SCREEN_H / 2));
                    base[count].attr1 = (ent->attr1.raw << 8) + (u8)(j * 2 * oamHeight + ent->pos.x + offX - ent->center.x * 2 - camera.x + (SCREEN_W / 2));
                    base[count].attr2 = ((ent->vramIndex + count) | ((ent->priority & 3) << 10) | ((ent->paletteIndex & 7) << 12));
                    count++;
                }
            } else {
                for (j = 0; j < oamWidth; j++) {
                    base[count].attr0 = (ent->attr0.raw << 8) + (u8)(i * 2 * oamHeight - ent->pos.y - offY - ent->center.y + camera.y + (SCREEN_H / 2));
                    base[count].attr1 = (ent->attr1.raw << 8) + (u8)(j * 2 * oamHeight + ent->pos.x + offX - ent->center.x - camera.x + (SCREEN_W / 2));
                    base[count].attr2 = ((ent->vramIndex + count) | ((ent->priority & 3) << 10) | ((ent->paletteIndex & 7) << 12));
                    count++;
                }
            }
        }
}

//---------------------------------------------------------------------------------
// Handle the keyframe the entity started with this frame.
//---------------------------------------------------------------------------------
const Keyframe* ParseAnimation(Entity* ent) {
    if (ent->animation != NULL && ent->animation->frames != NULL) {
        const Keyframe* frame;

        if (ent->frameIndex != 0xFF) {
            frame = &ent->animation->frames[ent->frameIndex];

            // here not lower because we dont want to flip every frame... unless?
            if (++ent->frameDuration < frame->duration) {
                return frame;
            } else {
                ent->frameDuration = 0;
            }

            if (frame->end) {
                if (frame->loop) {
                    ent->frameIndex = 0x00;
                }
                else {
                    ent->frameIndex = 0xFF;
                }
            }
            else {
                ent->frameIndex++;
            }
            return frame;
        }
    }
    return NULL;
}

//---------------------------------------------------------------------------------
// Updates any entity provided, as long as it has its object shape and OAM index
// assigned.
//---------------------------------------------------------------------------------
void UpdateObjectAttributes(Entity* ent) {
    OBJATTR* base = &OAMBuffer[ent->oamIndex];
    const Keyframe* curFrame;

    curFrame = ParseAnimation(ent);
    UpdateSprite(base, ent, curFrame);
}

void CopyPaletteBuffer() {
    dmaCopy(PaletteBuffer, SPRITE_PALETTE, sizeof(PaletteBuffer));
}

void CopyOAMBuffer() {
    dmaCopy(OAMBuffer, (u16*)OAM, sizeof(OAMBuffer));
}