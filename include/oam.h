#ifndef OAM_H
#define OAM_H

#include "global.h"
#include "entity.h"

extern OBJATTR OAMBuffer[128];

u32 Load24x32(const void* gfx);
//void Replace24x32(u32 index, OBJATTR* source);
void UpdateObjectAttributes(OBJATTR* gfx, Entity* ent, u32 objShape);
void CopyOAMBuffer();
#endif