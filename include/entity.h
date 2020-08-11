#ifndef ENTITY_H
#define ENTITY_H

#include "global.h"

// These help us determine how much space the entity requires in both OAM and VRAM
enum {
    NONE,
    SIZE_24x32
};

typedef struct {
    u8 oamAttr0;
    u8 oamAttr1;
    u16 oamIndex;
    u8 paletteIndex;
    u8 shape;
    u8 frameIndex;
    u8 animIndex;
    Pos pos;
    Pos vel;
} Entity;

Entity* CreateEntity();
void UpdateEntitiesOAM();
#endif