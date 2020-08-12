#include "global.h"
#include "oam.h"
#include "entity.h"

Entity EntStack[20];
u32 EntCount = 0;

extern void Player(Entity*);
extern void Shield(Entity*);

void (*const EntMains[])(Entity*) = {
    Player,
    Shield,
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

void ClearAllEntities() {
    int i;
    for (i = ARRAY_COUNT(EntStack); i > 0; i--) {
        dmaCopy(0x0, &EntStack[i], sizeof(EntStack[i]));
    }
}

//---------------------------------------------------------------------------------
// Pushes the entity's object attributes to the buffer.
//---------------------------------------------------------------------------------
void UpdateEntitiesOAM() {
    int i;
    for (i = 0; i < ARRAY_COUNT(EntStack); i++) {
        UpdateObjectAttributes(&EntStack[i]);
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