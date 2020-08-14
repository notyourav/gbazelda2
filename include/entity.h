#ifndef ENTITY_H
#define ENTITY_H

#include "global.h"

// These help us determine how much space the entity requires in both OAM and VRAM
enum {
    NONE,
    SIZE_16x16,
    SIZE_24x32
};

typedef struct {
    u32 duration : 8;
    u32 gfxOffset : 4;
    u32 flipX : 1; // NOTE: these flip the sprite from its current state.
    u32 flipY : 1;
    u32 xOff : 8;
    u32 yOff : 8;
    u32 loop : 1;
    u32 end : 1;
} Keyframe;

typedef struct {
    const u8* gfx;
    const Keyframe* frames;
} Animation;

typedef struct Entity {
    u8 type;
    u8 action;
    u16 oamIndex;
    u8 paletteIndex;
    const Animation* animation;
    u8 frameIndex;
    u8 frameDuration;
    u8 vramIndex;
    union {
        u8 raw;
        struct {
            u8 rotScaleFlag : 1;
            u8 ctxFlag : 1;
            u8 mode : 2;
            u8 mosaic : 1;
            u8 colorMode : 1;
            u8 shape : 2;
        } f;
    } attr0;
    union {
        struct {
            u8 reserved : 1;
            u8 scaleFlags : 3;
            u8 flipX : 1;
            u8 flipY : 1;
            u8 objSize : 2;
        } f;
        u8 raw;
    } attr1;
    u8 shape;
    u8 priority;
    Pos pos;
    Pos center;
    Pos vel;
    struct Entity* parent;
    struct Entity* child1;
    struct Entity* child2;
} Entity;

Entity* CreateEntity(u32 type);
Entity* FindEntity(u32 type);
void SetAnimation(Entity* ent, const Animation* animation);

void UpdateEntities();
void UpdateEntitiesOAM();

extern Entity EntStack[];
extern u32 EntCount;

#endif