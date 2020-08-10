#ifndef OAM_H
#define OAM_H

#include "global.h"

extern OBJATTR OAMBuffer[128];
u32 Load24x32(const void* gfx);
void Replace24x32(u32 index, OBJATTR* source);
void CopyOAMBuffer();
#endif