#include "global.h"
#include "sprite.h"
#include "entity.h"
#include "main.h"

OBJATTR OAMBuffer[128];
static u32 stackSize;

// in 8x8 bitmaps
u32 Load24x32(OBJATTR* gfx) {
    int i;
    for (i = 0; i < 12; i++) {
        OAMBuffer[stackSize + i] = gfx[i];
    }
    stackSize += 12;
    return stackSize - 12;
}

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

void UpdateObjectAttributes(u32 index, Entity* ent, u32 objShape) {
    OBJATTR* base = &OAMBuffer[index];

    switch (objShape) {
        case NONE:
            break;
        case SIZE_24x32:
            Update24x32(base, ent);
    }
}

void CopyOAMBuffer() {
    dmaCopy(OAMBuffer, (u16*)OAM, sizeof(OAMBuffer));
}