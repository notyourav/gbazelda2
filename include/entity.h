#ifndef ENTITY_H
#define ENTITY_H

#include "global.h"

// These help us determine how much space the entity requires in both OAM and VRAM
enum {
    NONE,
    SIZE_16x16,
    SIZE_24x32
};

typedef struct Entity {
    u8 type;
    u8 action;
    u16 oamIndex;
    u8 paletteIndex;
    u8 animIndex;
    u8 frameIndex;
    u8 vramIndex;
    u8 oamAttr0;
    u8 oamAttr1;
    u8 shape;
    Pos pos;
    Pos vel;
    struct Entity* parent;
    struct Entity* child1;
    struct Entity* child2;
} Entity;

Entity* CreateEntity(u32 type);
void UpdateEntities();
void UpdateEntitiesOAM();

extern Entity EntStack[];
extern u32 EntCount;

#endif