//---------------------------------------------------------------------------------
// GBA ZELDA 2 MAIN
//---------------------------------------------------------------------------------
#include "global.h"
#include "main.h"
#include "gba_console.h"
#include "amiga_1bpp.h"
#include "debug.h"
#include <stdio.h>
#include "tilemap.h"

u16 keysDownRaw;
Input inputDown;
u16 keysHeldRaw;
Input inputHeld;
u32 frame;
Pos camera;

//---------------------------------------------------------------------------------
// Interrupts and timing
//---------------------------------------------------------------------------------
void VblankInterrupt()
{
	frame++;
	scanKeys();
	keysDownRaw = keysDown();
	inputDown = *(Input *)&keysDownRaw;
	keysHeldRaw = keysHeld();
	inputHeld = *(Input *)&keysHeldRaw;

}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
{
	// Set up the interrupt handlers
	irqInit();
	irqSet(IRQ_VBLANK, VblankInterrupt);
	irqEnable(IRQ_VBLANK);

	// Allow Interrupts
	REG_IME = 1;

	SetMode( MODE_1 | BG0_ENABLE | BG0_ON | OBJ_ENABLE | OBJ_1D_MAP );
	camera.x = 0;
	camera.y = 0;
	
	CreateEntity(0); // player
	CreateEntity(1); // sword
	CreateEntity(2); // shield
	CreateEntity(3); // ori
	CreateEntity(4); // girl
	LoadTiles();

	while (1)
	{
		VBlankIntrWait();

		DebugScanKeys();
	
		UpdateEntities();
		UpdateEntitiesOAM();

		CopyPaletteBuffer();
		CopyOAMBuffer();
	}
}