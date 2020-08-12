#include "global.h"
#include "player.h"

enum {
    INIT,
    UPDATE,
};

static void Init();
static void Update();

static void DoPhysics();
static void UpdateAnimation();

static void (*const PlayerActions[])(Entity*) = {
    Init,
    Update,
};

void Player(Entity* this) {
    PlayerActions[this->action](this);
}

static void Init(Entity* this) {
    LoadSprite(link_run_4bpp, link_gbapal, this, SIZE_24x32);
    this->action = UPDATE;
}

static void Update(Entity* this) {
        if (inputDown.a) {
            this->vel.y += 120;
        }
        if (inputHeld.left) {
            this->pos.x -= 1;
        }
        if (inputHeld.right) {
            this->pos.x += 1;
        }
        if (inputHeld.up) {
            this->pos.y += 1;
        }
        if (inputHeld.down) {
            this->pos.y -= 1;
        }
        
        DoPhysics(this);
        UpdateAnimation();
}

u32 animFrame;
static void UpdateAnimation() {
    if (frame % 3 == 0) {
        animFrame = (animFrame + 1) % 12;
        dmaCopy(link_run_4bpp + (32 * 12 * animFrame), &MEM_TILE[4][0], 32 * 12);
    }
}

static void DoPhysics(Entity* this) {
        this->pos.y += this->vel.y >> 4;

        if (this->vel.y > -50) {
            this->vel.y -= 5;
        }
        else {
            this->vel.y = -50;
        }

        if (this->pos.y < 0) {
            this->pos.y = 0;
        }
}