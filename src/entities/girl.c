#include "global.h"
#include "entity.h"
#include "sprite.h"
#include "trig.h"

#include "girl_gbapal.h"
#include "girl_4bpp.h"

#include "tonc_nocash.h"
#include <stdio.h>

u32 fCount = 0;
u32 fTarget = 0;

enum {
    INIT,
    DO_IDLE,
    DO_WALK,
};

enum {
    NO_ANIM,
    IDLE,
    WALK,
};

static const Keyframe Idle[] = {
    { // FRAME 0
        .duration  = 16,
        .gfxOffset = 0,
        .loop      = true,
        .end       = true,
    },
};

static const Keyframe Walk[] = {
    { // FRAME 0
        .duration  = 5,
        .gfxOffset = 0,
    },
    { // FRAME 1
        .duration  = 5,
        .gfxOffset = 1,
    },
    { // FRAME 2
        .duration  = 5,
        .gfxOffset = 2,
        .loop      = true,
        .end       = true,
    },
};

static const Animation Animations[] = {
    {
        .gfx = girl_4bpp,
        .frames = NULL,
    },
    {
        .gfx = girl_4bpp,
        .frames = Idle,
    },
    {
        .gfx = girl_4bpp,
        .frames = Walk,
    },
};

static void Init(Entity*);
static void DoIdle(Entity*);
static void DoWalk(Entity*);

void (*const GirlActions[])(Entity*) = {
    Init,
    DoIdle,
    DoWalk,
};

void Girl(Entity* this) {
    GirlActions[this->action](this);
}

static void Init(Entity* this) {
    LoadSprite(girl_4bpp, girl_gbapal, this, SIZE_16x32);
    this->action = DO_WALK;
}

static void DoIdle(Entity* this) {
    if (fCount == 0) {
        SetAnimation(this, &Animations[IDLE]);
        fTarget = 4 + (random() & 31);
    } else if (fCount >= fTarget) {
        fCount = 0;
        this->action = DO_WALK;
        return;
    }

    if (this->frameDuration == 0) {
        fCount++;
    }
}

static void DoWalk(Entity* this) {
    if (fCount == 0) {
        if (random() % 2) {
            this->attr1.f.flipX = !this->attr1.f.flipX;
        }
        SetAnimation(this, &Animations[WALK]);
        fTarget = 200 + (random() & 7);
    } else if (fCount >= fTarget) {
        fCount = 0;
        this->action = DO_IDLE;
        return;
    }

    if (this->frameDuration == 0) {
        fCount++;
        this->pos.x += 2 * (this->attr1.f.flipX * 2 - 1);
    }
}