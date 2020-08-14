#include "global.h"
#include "entity.h"
#include "sprite.h"
#include "trig.h"

#include "girl_gbapal.h"
#include "girl_4bpp.h"

static void Init(Entity*);
static void DoIdle(Entity*);

enum {
    INIT,
    DO_IDLE,
};

void (*const GirlActions[])(Entity*) = {
    Init,
    DoIdle,
};

void Girl(Entity* this) {
    GirlActions[this->action](this);
}

static void Init(Entity* this) {
    LoadSprite(girl_4bpp, girl_gbapal, this, SIZE_16x32);
    this->action = DO_IDLE;
}

static void DoIdle(Entity* this) {
    this->pos.x = SineTable[(frame + 64) % 256] / 16;
    this->pos.y = SineTable[frame * 4 % 256] / 16;
}