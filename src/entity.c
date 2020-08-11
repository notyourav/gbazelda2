#include "global.h"
#include "entity.h"
#include "oam.h"

Entity entStack[20];
u32 entCount = 0;

Entity* CreateEntity() {
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

void UpdateEntitiesOAM() {
    int i;
    for (i = 0; i < ARRAY_COUNT(entStack); i++) {
        UpdateObjectAttributes(entStack[i].oamIndex, &entStack[i], entStack[i].shape );
    }
}