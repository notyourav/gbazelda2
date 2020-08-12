#include "global.h"
#include "entity.h"
#include "sprite.h"

#include "shield_4bpp.h"
#include "link_gbapal.h"

enum {
    INIT,
    UPDATE,
};

enum {
    NO_ANIM,
    RUN,
};

static const Keyframe Run[] = {
    {
        .duration = 1,
        .xOff = 0xff,
    },
    {
        .duration = 1,
        .xOff = 0xfe,
    },
    {
        .duration = 1,
        .xOff = 0xfd,
    },
    {
        .duration = 1,
        .xOff = 0xfe,
    },
    {
        .duration = 1,
        .xOff = 0xff,
    },
    {
        .duration = 1,
    },
    {
        .duration = 1,
    },
    {
        .duration = 1,
    },
    {
        .duration = 1,
    },
    {
        .duration = 1,
    },
    {
        .duration = 1,
    },
    {
        .duration = 1,
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
        .frames = Run,
    }
};

static void Init(Entity*);
static void Update(Entity*);

static void (*const ShieldActions[])(Entity*) = {
    Init,
    Update,
};

void Shield(Entity* this) {
    ShieldActions[this->action](this);
}

static void Init(Entity* this) {
    LoadSprite(shield_4bpp, NULL, this, SIZE_16x16);
    this->pos.y = -10;
    this->oamAttr1 = 0b01000000;
    this->parent = FindEntity(0);
    this->animation = &Animations[RUN];
    this->priority = 1;
    this->action = UPDATE;
}

static void Update(Entity* this) {

}