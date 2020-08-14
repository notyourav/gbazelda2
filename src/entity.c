#include "global.h"
#include "sprite.h"
#include "entity.h"

Entity EntStack[20];
u32 EntCount = 0;

extern void Player(Entity*);
extern void Sword(Entity*);
extern void Shield(Entity*);
extern void Ori(Entity*);

void (*const EntMains[])(Entity*) = {
    Player,
    Sword,
    Shield,
    Ori,
};


//---------------------------------------------------------------------------------
// Creates a new empty entity.
//---------------------------------------------------------------------------------
Entity* CreateEntity(u32 type) {
    if (EntCount < ARRAY_COUNT(EntStack)) {

        Entity* newEntity = &EntStack[EntCount];
        newEntity->type = type;
        EntCount++;
        return newEntity;
    }
    return NULL;
}

//---------------------------------------------------------------------------------
// Returns the first occurence of entity type.
//---------------------------------------------------------------------------------
Entity* FindEntity(u32 type) {
    int i;
    Entity* c;
    for (i = 0; i < EntCount; i++) {
        c = &EntStack[i];
        if (c->type == type) {
            return c;
        }
    }
    return NULL;
}

void SetAnimation(Entity* ent, const Animation* animation) {
    if (ent->animation != animation) {
        ent->animation = animation;
        ent->frameIndex = 0;
        ent->frameDuration = 0;
    }
}

//---------------------------------------------------------------------------------
// Remove all entities.
//---------------------------------------------------------------------------------
void ClearAllEntities() {
    int i;

    // Could be simplified to one dmaCopy, but this is a placeholder in case
    // we want to treat the clearing of entities differently.
    for (i = ARRAY_COUNT(EntStack); i > 0; i--) {
        // TODO: does this result in a forbidden memory access?
        dmaCopy(0x0, &EntStack[i], sizeof(EntStack[i]));
    }
}

//---------------------------------------------------------------------------------
// Pushes the entity's object attributes to the buffer.
//---------------------------------------------------------------------------------
void UpdateEntitiesOAM() {
    int i;
    Entity* c;
    for (i = 0; i < EntCount; i++) {
        c = &EntStack[i];
        UpdateObjectAttributes(c);
    }
}

//---------------------------------------------------------------------------------
// Update all entites.
//---------------------------------------------------------------------------------
void UpdateEntities() {
    int i;

    if (EntCount == 0) return;

    for (i = 0; i < EntCount; i++) {
        EntMains[EntStack[i].type](&EntStack[i]);
    }
}