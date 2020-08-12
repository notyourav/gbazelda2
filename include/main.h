#ifndef MAIN_H
#define MAIN_H

#include "global.h"
#include "player.h"
#include "sprite.h"
#include "entity.h"

typedef struct {
    u16 a      : 1;
    u16 b      : 1;
    u16 select : 1;
    u16 start  : 1;
    u16 right  : 1;
    u16 left   : 1;
    u16 up     : 1;
    u16 down   : 1;
    u16 rTrig  : 1;
    u16 lTrig  : 1;
    u16 dummy  : 6;
} __attribute__((__may_alias__)) Input;

// Generally this would be a no-no and a macro should be used instead,
// but this servers two purposes; storing a copy of the keys pressed
// *and* making them human readable.

extern u16 keysDownRaw;
extern Input inputDown;
extern u16 keysHeldRaw;
extern Input inputHeld;
extern u32 frame;
extern Pos camera;

#endif