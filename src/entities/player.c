#include "global.h"
#include "main.h"
#include "sprite.h"
#include "entity.h"
#include "gba_systemcalls.h"
#include "player.h"

#include "link_idle_4bpp.h"
#include "link_run_4bpp.h"
#include "link_chop_4bpp.h"
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
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Run[] = {
    {
        .duration = 2,
        .gfxOffset = 0,
    },
    {
        .duration = 2,
        .gfxOffset = 1,
    },
    {
        .duration = 2,
        .gfxOffset = 2,
    },
    {
        .duration = 2,
        .gfxOffset = 3,
    },
    {
        .duration = 2,
        .gfxOffset = 4,
    },
    {
        .duration = 2,
        .gfxOffset = 5,
    },
    {
        .duration = 2,
        .gfxOffset = 6,
    },
    {
        .duration = 2,
        .gfxOffset = 7,
    },
    {
        .duration = 2,
        .gfxOffset = 8,
    },
    {
        .duration = 2,
        .gfxOffset = 9,
    },
    {
        .duration = 2,
        .gfxOffset = 10,
    },
    {
        .duration = 2,
        .gfxOffset = 11,
        .loop = 1,
        .end = 1,
    },
};

static const Keyframe Chop[] = {
    {
        .duration = 2,
        .gfxOffset = 0,
    },
    {
        .duration = 2,
        .gfxOffset = 1,
    },
    {
        .duration = 2,
        .gfxOffset = 2,
    },
    {
        .duration = 2,
        .gfxOffset = 3,
    },
    {
        .duration = 2,
        .gfxOffset = 4,
    },
    {
        .duration = 2,
        .gfxOffset = 5,
    },
    {
        .duration = 2,
        .gfxOffset = 6,
    },
    {
        .duration = 2,
        .gfxOffset = 7,
    },
    {
        .duration = 2,
        .gfxOffset = 8,
    },
    {
        .duration = 2,
        .gfxOffset = 9,
        .loop = 0,
        .end = 1,
    },
};

static const Animation Animations[] = {
    {
        .gfx = link_idle_4bpp,
        .frames = NULL,
    },
    {
        .gfx = link_idle_4bpp,
        .frames = Idle,
    },
    {
        .gfx = link_run_4bpp,
        .frames = Run,
    },
    {
        .gfx = link_chop_4bpp,
        .frames = Chop,
    },
};

static void Init(Entity*);
static void DoIdle(Entity*);
static void DoRun(Entity*);
static void DoChop(Entity*);

static void ParseKeyInput(Entity*);
static void HandleMovement(Entity*);

static void (*const PlayerActions[])(Entity*) = {
    Init,
    DoIdle,
    DoRun,
    DoChop,
};

void Player(Entity* this) {
    PlayerActions[this->action](this);
    HandleMovement(this);
}

static void Init(Entity* this) {
    LoadSprite(link_run_4bpp, link_gbapal, this, SIZE_24x32);
    this->priority = 1;
    SetAnimation(this, &Animations[IDLE]);
    this->action = DO_IDLE;
}

static void ParseKeyInput(Entity* this) {
    if (inputDown.a) {
        this->vel.y += 120;
    }

    if (inputDown.b) {
        SetAnimation(this, &Animations[CHOP]);
        this->action = DO_CHOP;
    }
    else if (inputHeld.left) {
        this->pos.x -= (frame & 1) * 3;
        SetAnimation(this, &Animations[RUN]);
        this->attr1.f.flipX = false;
        this->action = DO_RUN;
    }
    else if (inputHeld.right) {
        this->pos.x += (frame & 1) * 3;
        SetAnimation(this, &Animations[RUN]);
        this->attr1.f.flipX = true;
        this->action = DO_RUN;
    }
    else {
        SetAnimation(this, &Animations[IDLE]);
        this->action = DO_IDLE;
        this->frameIndex = 0;
    }
}

static void DoIdle(Entity* this) {
    ParseKeyInput(this);
}

static void DoRun(Entity* this) {
    ParseKeyInput(this);
}

static void DoChop(Entity* this) {
    if (this->frameIndex == 0xFF) {
        this->frameIndex = 0x00;
        ParseKeyInput(this);
    }
}

static void HandleMovement(Entity* this) {
        this->pos.y += this->vel.y >> 4;

        this->pos.x += this->vel.x >> 4;

        if (this->vel.y > -50) {
            this->vel.y -= 5;
        }
        else {
            this->vel.y = -50;
        }

        if (this->pos.y < 0) {
            this->pos.y = 0;
        }

        if (camera.x - this->pos.x < -20) {
            camera.x += 2;
        }
        else if (camera.x - this->pos.x > 20) {
            camera.x -= 2;
        }
}