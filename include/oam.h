#ifndef OAM_H
#define OAM_H

#include "global.h"
#include "entity.h"

extern OBJATTR OAMBuffer[128];

void Load24x32(OBJATTR* gfx, const u8* palette, Entity* ent);
void UpdateObjectAttributes(Entity* ent);
void CopyPaletteBuffer();
void CopyOAMBuffer();
#endif