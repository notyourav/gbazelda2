#include "global.h"
#include "player.h"

OBJATTR* attr;
Entity* this;

u32 oamIndex = 0;

void SetupPlayer() {
    this = CreateEntity();
    this->oamIndex = Load24x32(link_run_4bpp);
    this->paletteIndex = 0;
	dmaCopy(link_run_gbapal, SPRITE_PALETTE, link_run_gbapal_size);
    this->shape = SIZE_24x32;
    // int i, j, c = 0;
    // for (i = 0; i < 4; i++) {  
    //     for (j = 0; j < 3; j++) {
    //         OAMBuffer[oamIndex + c].attr0 = 0b0000000000000000 + ((i * 8) & 0x1ff);
    //         OAMBuffer[oamIndex + c].attr1 = 0b0000000000000000 + ((j * 8) & 0x1ff);
    //         OAMBuffer[oamIndex + c].attr2 = c;
    //         c++;
    //     }
    // }
}

// void SetTilesPos(s16 x, s16 y) {
//     int i, j, c = 0;
//     for (i = 0; i < 4; i++) {  
//         for (j = 0; j < 3; j++) {
//             OAMBuffer[oamIndex + c].attr0 = ((OAMBuffer[oamIndex + c].attr0 >> 8) << 8) + (u8)((i * 8 - y + camera.y + 90) & 0x1ff);
//             OAMBuffer[oamIndex + c].attr1 = ((OAMBuffer[oamIndex + c].attr1 >> 8) << 8) + (u8)((j * 8 + x - camera.x + 120) & 0x1ff);
//             //OAMBuffer[oamIndex + c].attr2 = c;
//             c++;
//         }
//     }
// }

void TranslateTiles(s16 x, s16 y) {
    int i, j, c = 0;
    for (i = 0; i < 4; i++) {  
        for (j = 0; j < 3; j++) {
            u8 dX, dY;
            dX = ((((OAMBuffer[oamIndex + c].attr1 << 8) >> 8) + x - camera.x + 120) & 0x1ff);
            dY = ((((OAMBuffer[oamIndex + c].attr0 << 8) >> 8) - y + camera.y + 90) & 0x1ff);
            OAMBuffer[oamIndex + c].attr0 = ((OAMBuffer[oamIndex + c].attr0 >> 8) << 8) + dY;
            OAMBuffer[oamIndex + c].attr1 = ((OAMBuffer[oamIndex + c].attr1 >> 8) << 8) + dX;
            //OAMBuffer[oamIndex + c].attr2 = c;
            c++;
        }
    }
}

u32 animFrame;
void UpdateAnimation() {
    //Replace24x32(oamIndex, &link_cycle_4bpp[64]);
    if (frame % 3 == 0) {
        animFrame = (animFrame + 1) % 12;
        dmaCopy(link_run_4bpp + (32 * 12 * animFrame), &MEM_TILE[4][0], 32 * 12);

    }
}

// void UpdateSpritePositions() {
//     SetTilesPos(this->pos.x, this->pos.y);
// }

void CleanUpMovement() {
        this->pos.y += this->vel.y >> 4;

        if (this->vel.y > -50) {
            this->vel.y -= 5;
        }
        else {
            this->vel.y = -50;
        }

        if (this->pos.y < 0) {
            this->pos.y = 0;
        }
}

void PlayerLoop() {
        //dmaCopy(link_cycle_4bpp[1 * (frame & 0x30 + 1)], &MEM_TILE[4][0], 64);
        if (inputDown.a) {
            this->vel.y += 120;
        }
        if (inputHeld.left) {
            this->pos.x -= 1;
        }
        if (inputHeld.right) {
            this->pos.x += 1;
        }
        if (inputHeld.up) {
            this->pos.y += 1;
        }
        if (inputHeld.down) {
            this->pos.y -= 1;
        }
        
        CleanUpMovement();
        UpdateAnimation();
        UpdateEntitiesOAM();
        //UpdateSpritePositions();
        //attr->attr0 = ((attr->attr0 >> 8) << 8) + (((-this.pos.y + camera.y + 90) & 0x1ff));
		//attr->attr1 = ((attr->attr1 >> 8) << 8) + (((this.pos.x + camera.x + 120) & 0x1ff));
}