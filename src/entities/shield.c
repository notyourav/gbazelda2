#include "global.h"
#include "entity.h"
#include "sprite.h"

#include "shield_4bpp.h"
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
        .xOff = -4,
        .yOff = -4,
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Run[] = {
    { // FRAME 0
        .duration = 2,
        .xOff = -3,
        .yOff = 0xfb,
    },
    { // FRAME 1
        .duration = 2,
        .xOff = -3,
        .yOff = 0xfb,
    },
    { // FRAME 2
        .duration = 2,
        .xOff = -3,
        .yOff = 0xfa,
    },
    { // FRAME 3
        .duration = 2,
        .xOff = -3,
        .yOff = 0xfa,
    },
    { // FRAME 4
        .duration = 2,
        .xOff = -4,
        .yOff = 0xfb,
    },
    { // FRAME 5
        .duration = 2,
        .xOff = -6,
        .yOff = 0xfa,
    },
    { // FRAME 6
        .duration = 2,
        .xOff = -6,
        .yOff = 0xfb,
    },
    { // FRAME 7
        .duration = 2,
        .xOff = -5,
        .yOff = 0xfb,
    },
    { // FRAME 8
        .duration = 2,
        .xOff = -3,
        .yOff = 0xfb,
    },
    { // FRAME 9
        .duration = 2,
        .xOff = -3,
        .yOff = 0xfa,
    },
    { // FRAME 10
        .duration = 2,
        .xOff = -5,
        .yOff = 0xfa,
    },
    { // FRAME 11 OK
        .duration = 2,
        .xOff = -5,
        .yOff = 0xfb,
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Chop[] = {
    {
        .duration = 1,
        .xOff = -4,
        .yOff = -4,
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
    {
        .gfx = shield_4bpp,
        .frames = Chop,
    }
};

static void Init(Entity*);
static void DoIdle(Entity*);
static void DoRun(Entity*);
static void DoChop(Entity*);

static void (*const ShieldActions[])(Entity*) = {
    Init,
    DoIdle,
    DoRun,
    DoChop,
};

void Shield(Entity* this) {
    if (this->parent != NULL && this->parent->action < ARRAY_COUNT(ShieldActions)) {
        this->action = this->parent->action;
    }
    ShieldActions[this->action](this);

    if (this->parent->attr1.f.flipX == true) {
        this->pos.x = -1;
    }
    else {
        this->pos.x = 0;
    }

    if (this->parent != NULL) {
        this->frameIndex = this->parent->frameIndex;
        this->attr1.f.flipX = this->parent->attr1.f.flipX;
    }
}

static void Init(Entity* this) {
    LoadSprite(shield_4bpp, NULL, this, SIZE_16x16);
    this->attr1.raw = 0b01000000;
    this->parent = FindEntity(0);
    SetAnimation(this, &Animations[NO_ANIM]);
    this->priority = 1;
    this->action = DO_IDLE;
}

static void DoIdle(Entity* this) {
    SetAnimation(this, &Animations[IDLE]);
}

static void DoRun(Entity* this) {
    SetAnimation(this, &Animations[RUN]);
}

static void DoChop(Entity* this) {
    SetAnimation(this, &Animations[CHOP]);
}