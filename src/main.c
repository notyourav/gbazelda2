//---------------------------------------------------------------------------------
// GBA ZELDA 2 MAIN
//---------------------------------------------------------------------------------
#include "global.h"
#include "main.h"
#include "gba_console.h"
#include "amiga_1bpp.h"
#include "debug.h"

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
	frame += 1;
	scanKeys();
	keysDownRaw = keysDown();
	inputDown = *(Input *)&keysDownRaw;
	keysHeldRaw = keysHeld();
	inputHeld = *(Input *)&keysHeldRaw;
}

inline void vsync()
{
    while (REG_VCOUNT >= 160);
    while (REG_VCOUNT < 160);
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

	SetMode( MODE_1 | BG0_ENABLE | OBJ_ENABLE | OBJ_1D_MAP );
	camera.x = 0;
	camera.y = 0;
	//consoleInit(0, 31, 0, amiga_1bpp, 10, 0);
	
	CreateEntity(0);
	CreateEntity(1);
	CreateEntity(2);
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