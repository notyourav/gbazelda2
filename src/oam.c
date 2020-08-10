#include "global.h"
#include "sprite.h"

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

void Replace24x32(u32 index, OBJATTR* source) {
    int i;
    for (i = 0; i < 12; i++) {
        OAMBuffer[index + i] = source[i];
    }
}

void CopyOAMBuffer() {
    dmaCopy(OAMBuffer, (u16*)OAM, sizeof(OAMBuffer));
}