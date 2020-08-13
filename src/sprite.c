#include "global.h"
#include "sprite.h"
#include "tonc_nocash.h"
#include <stdio.h>

OBJATTR OAMBuffer[128];
static u32 objectCount = 0;

// each tile is a word, 8x8 pixels
u32 tileCount = 0;

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
                dmaCopy(gfx, SPR_VRAM(tileCount), 32 * 4);
                ent->center.x = 8;
                ent->center.y = 8;
                tileCount += 4;
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
        dmaCopy(palette, &PaletteBuffer[paletteCount], 32);
        ent->paletteIndex = paletteCount;
        paletteCount++;
    }

    ent->oamIndex = objectCount;
    ent->shape = shape;

    objectCount += objNum;
}

void Update16x16(OBJATTR* base, Entity* ent, const Keyframe* curFrame) {
    int offX = 0;
    int offY = 0;

    if (curFrame != NULL) {
        if (ent->attr1.f.flipX) {
            offX += curFrame->xOff;
        }
        else {
            offX -= curFrame->xOff;
        }
        offY += curFrame->yOff;
    	//sprintf(nocash_buffer, "VRAM INDEX: %u", ent->vramIndex);
		//nocash_message();
        dmaCopy(ent->animation->gfx + (curFrame->gfxOffset * (32 * 4)), SPR_VRAM(ent->vramIndex), 32 * 4);
    }

    if (ent->parent != NULL) {
        offX += ent->parent->pos.x;
        offY += ent->parent->pos.y;
    }

    base[0].attr0 = (ent->attr0.raw << 8) + (u8)(-ent->pos.y - offY - ent->center.y + camera.y + (SCREEN_H / 2));
    base[0].attr1 = (ent->attr1.raw << 8) + (u8)( ent->pos.x + offX - ent->center.x - camera.x + (SCREEN_W / 2));
    base[0].attr2 = ent->vramIndex | ((ent->priority & 3) << 10);
}

//---------------------------------------------------------------------------------
// Updates every tile for a 24x32 sized entity.
//---------------------------------------------------------------------------------
void Update24x32(OBJATTR* base, Entity* ent, const Keyframe* curFrame) {
    int i = 0;
    int j = 0;
    int c = 0;
    int offX = 0;
    int offY = 0;

    if (curFrame != NULL) {
        offX += curFrame->xOff;
        offY += curFrame->yOff;
        dmaCopy(ent->animation->gfx + (curFrame->gfxOffset * (32 * 12)), SPR_VRAM(ent->vramIndex), 32 * 12);
    }

    if (ent->parent != NULL) {
        offX += ent->parent->pos.x;
        offY += ent->parent->pos.y;
    }

    for (i = 0; i < 4; i++) {  
        if (((base[c].attr1 >> 8) & 0x10) != ent->attr1.f.flipX) {
            for (j = 3; j > 0; j--) {
                base[c].attr0 = (ent->attr0.raw << 8) + (u8)(i * 8 - ent->pos.y - offY - ent->center.y + camera.y + (SCREEN_H / 2));
                base[c].attr1 = (ent->attr1.raw << 8) + (u8)(j * 8 + ent->pos.x + offX - ent->center.x * 2 - camera.x + (SCREEN_W / 2));
                base[c].attr2 = (ent->vramIndex + c) | ((ent->priority & 3) << 10);
                c++;
            }
        }
        else {
            for (j = 0; j < 3; j++) {
                base[c].attr0 = (ent->attr0.raw << 8) + (u8)(i * 8 - ent->pos.y - offY - ent->center.y + camera.y + (SCREEN_H / 2));
                base[c].attr1 = (ent->attr1.raw << 8) + (u8)(j * 8 + ent->pos.x + offX - ent->center.x - camera.x + (SCREEN_W / 2));
                base[c].attr2 = (ent->vramIndex + c) | ((ent->priority & 3) << 10);
                c++;
            }
        }
    }
}

const Keyframe* ParseAnimation(Entity* ent) {
    if (ent->animation != NULL && ent->animation->frames != NULL) {
        const Keyframe* frame;

        if (ent->frameIndex != 0xFF) {
            frame = &ent->animation->frames[ent->frameIndex];
            if (frame->flipX) {
                ent->attr1.f.flipX = !ent->attr1.f.flipX;
            }

            if (frame->flipY) {
                ent->attr1.f.flipY = !ent->attr1.f.flipY;
            }

            if (ent->frameDuration < frame->duration) {
                ent->frameDuration++;
                return frame;
            }
            else {
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

    switch (ent->shape) {
        case NONE:
            break;
        case SIZE_16x16:
            Update16x16(base, ent, curFrame);
            break;
        case SIZE_24x32:
            Update24x32(base, ent, curFrame);
            break;
    }
}

void CopyPaletteBuffer() {
    dmaCopy(PaletteBuffer, SPRITE_PALETTE, sizeof(PaletteBuffer));
}

void CopyOAMBuffer() {
    dmaCopy(OAMBuffer, (u16*)OAM, sizeof(OAMBuffer));
}