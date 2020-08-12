#include "global.h"
#include "entity.h"
#include "oam.h"

#include "shield_4bpp.h"
#include "link_gbapal.h"

enum {
    INIT,
    UPDATE,
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
    this->oamAttr1 = 0b01000000;
    this->action = UPDATE;
}

static void Update(Entity* this) {

}