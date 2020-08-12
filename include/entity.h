#ifndef ENTITY_H
#define ENTITY_H

#include "global.h"
#include "player.h"

// These help us determine how much space the entity requires in both OAM and VRAM
enum {
    NONE,
    SIZE_16_16,
    SIZE_24x32
};

typedef struct {
    u8 type;
    u8 action;
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

Entity* CreateEntity(u32 type);
void UpdateEntities();
void UpdateEntitiesOAM();
#endif