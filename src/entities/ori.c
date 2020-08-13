#include "global.h"
#include "entity.h"
#include "sprite.h"

#include "ori_4bpp.h"
#include "link_gbapal.h"

enum {
    INIT,
    UPDATE,
};

static void Init(Entity*);
static void Update(Entity*);

static void (*const OriActions[])(Entity*) = {
    Init,
    Update,
};

void Ori(Entity* this) {
    OriActions[this->action](this);
}

static void Init(Entity* this) {
    LoadSprite(ori_4bpp, NULL, this, SIZE_16x16);
    this->attr1.raw = 0b01000000;
    //this->pos.x = 10;
    //this->parent = FindEntity(0);
    this->priority = 3;
    this->action = UPDATE;
}

static void Update(Entity* this) {
    //this->attr1.f.flipX = this->parent->attr1.f.flipX;
}