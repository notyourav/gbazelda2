#include "global.h"
#include "oam.h"
#include "entity.h"


Entity entStack[20];
u32 entCount = 0;

void (*const EntMains[])(Entity*) = {
    Player,
};


//---------------------------------------------------------------------------------
// Creates a new empty entity.
//---------------------------------------------------------------------------------
Entity* CreateEntity(u32 type) {
    if (entCount < ARRAY_COUNT(entStack)) {
        entCount++;

        Entity* newEntity = &entStack[entCount];
        return newEntity;
    }
    return NULL;
}

void ClearAllEntities() {
    int i;
    for (i = ARRAY_COUNT(entStack); i > 0; i--) {
        dmaCopy(0x0, &entStack[i], sizeof(entStack[i]));
    }
}

//---------------------------------------------------------------------------------
// Pushes the entity's object attributes to the buffer.
//---------------------------------------------------------------------------------
void UpdateEntitiesOAM() {
    int i;
    for (i = 0; i < ARRAY_COUNT(entStack); i++) {
        UpdateObjectAttributes(&entStack[i]);
    }
}

//---------------------------------------------------------------------------------
// Update all entites.
//---------------------------------------------------------------------------------
void UpdateEntities() {
    int i;

    if (entCount == 0) return;

    for (i = 0; i < entCount; i++) {
        EntMains[entStack[i].type](&entStack[i]);
    }
}