#include "global.h"
#include "entity.h"
#include "sprite.h"

#include "shield_4bpp.h"
#include "link_gbapal.h"

enum {
    INIT,
    DO_IDLE,
    DO_RUN,
};

enum {
    NO_ANIM,
    IDLE,
    RUN,
};

static const Keyframe Idle[] = {
    {
        .duration = 1,
        .xOff = 4,
        .yOff = -4,
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Run[] = {
    { // FRAME 1
        .duration = 2,
        .xOff = 0x3,
        .yOff = 0xfb,
    },
    { // FRAME 2
        .duration = 2,
        .xOff = 0x3,
        .yOff = 0xfb,
    },
    { // FRAME 2
        .duration = 2,
        .xOff = 0x3,
        .yOff = 0xfa,
    },
    { // FRAME 3
        .duration = 2,
        .xOff = 0x3,
        .yOff = 0xfa,
    },
    { // FRAME 4
        .duration = 2,
        .xOff = 0x4,
        .yOff = 0xfb,
    },
    { // FRAME 5
        .duration = 2,
        .xOff = 0x6,
        .yOff = 0xfa,
    },
    { // FRAME 6
        .duration = 2,
        .xOff = 0x6,
        .yOff = 0xfb,
    },
    { // FRAME 7
        .duration = 2,
        .xOff = 0x5,
        .yOff = 0xfb,
    },
    { // FRAME 8
        .duration = 2,
        .xOff = 0x3,
        .yOff = 0xfb,
    },
    { // FRAME 9
        .duration = 2,
        .xOff = 0x3,
        .yOff = 0xfa,
    },
    { // FRAME 10
        .duration = 2,
        .xOff = 0x5,
        .yOff = 0xfa,
    },
    { // FRAME 11 OK
        .duration = 2,
        .xOff = 0x5,
        .yOff = 0xfb,
        .loop = 1,
        .end = 1,
    },
};

static const Animation Animations[] = {
    {
        .gfx = shield_4bpp,
        .frames = NULL,
    },
    {
        .gfx = shield_4bpp,
        .frames = Idle,
    },
    {
        .gfx = shield_4bpp,
        .frames = Run,
    },
};

static void Init(Entity*);
static void DoIdle(Entity*);
static void DoRun(Entity*);

static void (*const ShieldActions[])(Entity*) = {
    Init,
    DoIdle,
    DoRun,
};

void Shield(Entity* this) {
    ShieldActions[this->action](this);
    this->action = this->parent->action;

    if (this->parent->attr1.f.flipX == true) {
        this->pos.x = -1;
    }
    else {
        this->pos.x = 0;
    }
    this->frameIndex = this->parent->frameIndex;
    this->attr1.f.flipX = this->parent->attr1.f.flipX;
}

static void Init(Entity* this) {
    LoadSprite(shield_4bpp, NULL, this, SIZE_16x16);
    this->attr1.raw = 0b01000000;
    this->parent = FindEntity(0);
    this->animation = &Animations[NO_ANIM];
    this->priority = 1;
    this->action = DO_IDLE;
}

static void DoIdle(Entity* this) {
    this->animation = &Animations[IDLE];
}

static void DoRun(Entity* this) {
    this->animation = &Animations[RUN];
}