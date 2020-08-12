#include "global.h"
#include "debug.h"
#include <stdio.h>

#define BINPATTERN "%c%c%c%c%c%c%c%c"
#define BIN(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void DebugPrintEntityStack() {
    int i = 0;
    Entity* c = NULL;

    for (i = 0; i < EntCount; i++) {
        c = &EntStack[i];
        sprintf(nocash_buffer, "Entity %u", i);
        nocash_message();
        sprintf(nocash_buffer, "---------");
        nocash_message();
        sprintf(nocash_buffer, "type: %u", c->type);
        nocash_message();
        sprintf(nocash_buffer, "action: %u", c->action);
        nocash_message();
        sprintf(nocash_buffer, "oamIndex: %u", c->oamIndex);
        nocash_message();
        sprintf(nocash_buffer, "oamIndex: %u", c->oamIndex);
        nocash_message();
        sprintf(nocash_buffer, "oamAttr0: 0b"BINPATTERN, BIN(c->oamAttr0));
        nocash_message();
        sprintf(nocash_buffer, "oamAttr1: 0b"BINPATTERN, BIN(c->oamAttr1));
        nocash_message();
        sprintf(nocash_buffer, "shape: %u", c->shape);
        nocash_message();
        sprintf(nocash_buffer, "pos: %u, %u", c->pos.x, c->pos.y);
        nocash_message();
    }
}

const char conpal[8] = {
     ' ', '.', '-', '+', '*', '$', '#', '@'
};

int tileOffset;

void DebugPrintVRAM() {
    int i, j = 0;
    u8* spr = (u8 *)SPR_VRAM(0);
    char x, y;

    nocash_puts("\n\n\n");
    nocash_message();
    if (inputHeld.down) {
        tileOffset -= 8;
    }
    if (inputHeld.up) {
        tileOffset += 8;
    }
    if (tileOffset < 0) {
        tileOffset = 0;
    }

    spr += tileOffset;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 4; j++) {
            x = conpal[((*spr & 0xf) >> 1) % 8];
            y = conpal[((*spr & 0xf0) >> 9) % 8];
            nocash_buffer[j * 2] = x;
            nocash_buffer[j * 2 + 1] = y;
            spr++;
        }

        spr += 32;
        for (j = 0; j < 4; j++) {
            x = conpal[((*spr & 0xf) >> 1) % 8];
            y = conpal[((*spr & 0xf0) >> 9) % 8];
            nocash_buffer[j * 2 + 8] = x;
            nocash_buffer[j * 2 + 9] = y;
            spr++;
        }
        spr -= 36;
        nocash_message();
    }
}

void DebugScanKeys() {
    if (inputDown.rTrig) {
        DebugPrintEntityStack();
    } else if (inputHeld.lTrig) {
        //DebugPrintVRAM();
    }
}