#ifndef OAM_H
#define OAM_H

#include "global.h"
#include "main.h"
#include "entity.h"

extern OBJATTR OAMBuffer[128];

void LoadSprite(const u8* gfx, const u8* palette, Entity* ent, u32 shape);
void UpdateObjectAttributes(Entity* ent);
void CopyPaletteBuffer();
void CopyOAMBuffer();
#endif