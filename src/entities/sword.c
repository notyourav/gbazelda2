#include "global.h"
#include "entity.h"
#include "sprite.h"

#include "sword_4bpp.h"
#include "link_gbapal.h"

enum {
    INIT,
    UPDATE,
};

static void Init(Entity*);
static void Update(Entity*);

static void (*const SwordActions[])(Entity*) = {
    Init,
    Update,
};

void Sword(Entity* this) {
    SwordActions[this->action](this);
}

static void Init(Entity* this) {
    LoadSprite(sword_4bpp, NULL, this, SIZE_16x16);
    this->pos.x = 20;
    this->pos.y = -15;
    this->oamAttr1 = 0b01000000;
    this->parent = FindEntity(0);
    this->priority = 1;
    this->action = UPDATE;
}

static void Update(Entity* this) {

}