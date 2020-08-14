#include "global.h"
#include "entity.h"
#include "sprite.h"

#include "sword_4bpp.h"
#include "sword_idle_4bpp.h"
#include "sword_chop_4bpp.h"

#include "link_gbapal.h"

enum {
    INIT,
    DO_IDLE,
    DO_RUN,
    DO_CHOP,
};

enum {
    NO_ANIM,
    IDLE,
    RUN,
    CHOP,
};

static const Keyframe Idle[] = {
    {
        .duration = 1,
        .xOff = 6,
        .yOff = 8,
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Run[] = {
    { // FRAME 0
        .duration = 2,
        .xOff = 17,
        .yOff = 0xfb,
    },
    { // FRAME 1
        .duration = 2,
        .xOff = 17,
        .yOff = 0xfb,
    },
    { // FRAME 2
        .duration = 2,
        .xOff = 17,
        .yOff = 0xfa,
    },
    { // FRAME 3
        .duration = 2,
        .xOff = 17,
        .yOff = 0xfa,
    },
    { // FRAME 4
        .duration = 2,
        .xOff = 14,
        .yOff = 0xfa,
    },
    { // FRAME 5
        .duration = 2,
        .xOff = 16,
        .yOff = 0xfc,
    },
    { // FRAME 6
        .duration = 2,
        .xOff = 16,
        .yOff = 0xfd,
    },
    { // FRAME 7
        .duration = 2,
        .xOff = 16,
        .yOff = 0xfa,
    },
    { // FRAME 8
        .duration = 2,
        .xOff = 16,
        .yOff = 0xfb,
    },
    { // FRAME 9
        .duration = 2,
        .xOff = 16,
        .yOff = 0xfa,
    },
    { // FRAME 10
        .duration = 2,
        .xOff = 17,
        .yOff = 0xfc,
    },
    { // FRAME 11
        .duration = 2,
        .xOff = 17,
        .yOff = 0xfc,
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Chop[] = {
    { // FRAME 0
        .duration = 2,
        .gfxOffset = 2,
        .xOff = 6,
        .yOff = 8,
    },
    { // FRAME 1
        .duration = 2,
        .gfxOffset = 3,
        .xOff = 14,
        .yOff = 8,
    },
    { // FRAME 2
        .duration = 2,
        .gfxOffset = 0,
        .xOff = 20,
        .yOff = 4,
    },
    { // FRAME 3
        .duration = 2,
        .gfxOffset = 3,
        .flipY = true,
        .xOff = 9,
        .yOff = 15,
    },
    { // FRAME 4
        .duration = 2,
        .gfxOffset = 1,
        .flipY = true,
        .xOff = -4,
        .yOff = 15,
    },
    { // FRAME 5
        .duration = 2,
        .gfxOffset = 1,
        .flipX = true,
        .xOff = 10,
        .yOff = 15,
    },
    { // FRAME 6
        .duration = 2,
        .flipX = true,
        .gfxOffset = 0,
        .xOff = 14,
        .yOff = -2,
    },
    { // Frame 7
        .duration = 2,
        .flipX = true,
        .gfxOffset = 0,
        .xOff = 14,
        .yOff = -5,
    },
    { // Frame 8
        .duration = 2,
        .gfxOffset = 0,
        .flipX = true,
        .xOff = 4,
        .yOff = -6,
    },
    { // Frame 9
        .duration = 2,
        .gfxOffset = 3,
        .flipX = true,
        .xOff = 4,
        .yOff = 5,
    },
};

static const Animation Animations[] = {
    {
        .gfx = sword_4bpp,
        .frames = NULL,
    },
    {
        .gfx = sword_idle_4bpp,
        .frames = Idle,
    },
    {
        .gfx = sword_4bpp,
        .frames = Run,
    },
    {
        .gfx = sword_chop_4bpp,
        .frames = Chop,
    },
};

static void Init(Entity*);
static void DoIdle(Entity*);
static void DoRun(Entity*);
static void DoChop(Entity*);

static void (*const SwordActions[])(Entity*) = {
    Init,
    DoIdle,
    DoRun,
    DoChop,
};

void Sword(Entity* this) {
    if (this->parent != NULL && this->parent->action < ARRAY_COUNT(SwordActions)) {
        this->action = this->parent->action;
    }
    SwordActions[this->action](this);

    if (this->parent->attr1.f.flipX == true) {
        this->pos.x = -1;
    }
    else {
        this->pos.x = 0;
    }

    if (this->parent != NULL) {
        //this->frameIndex = this->parent->frameIndex;
    }

    if (this->action != CHOP) {
        this->attr1.f.flipX = this->parent->attr1.f.flipX;
    }
}

static void Init(Entity* this) {
    LoadSprite(sword_4bpp, NULL, this, SIZE_16x16);
    this->attr1.raw = 0b01000000;
    this->parent = FindEntity(0);
    SetAnimation(this, &Animations[IDLE]);
    this->priority = 1;
    this->action = DO_IDLE;
}

static void DoIdle(Entity* this) {
    //this->frameIndex = 0;
    SetAnimation(this, &Animations[IDLE]);
}

static void DoRun(Entity* this) {
    SetAnimation(this, &Animations[RUN]);
}

static void DoChop(Entity* this) {
    SetAnimation(this, &Animations[CHOP]);
}